#ifndef PROTOBUFDECODER_H
#define PROTOBUFDECODER_H

#include <cstddef>
#include <cstdint>
#include <vector>

// Minimal protobuf wire-format reader for Cowboy dashboard notifications.
// It preserves the wire type and raw value; callers interpret known fields.
namespace protobuf {
struct Field {
    uint32_t number;
    uint32_t wireType;
    uint64_t value;
};

inline bool varint(const uint8_t *data, size_t size, size_t &offset, uint64_t &result)
{
    result = 0;
    for (unsigned i = 0; i < 10 && offset < size; ++i) {
        const uint8_t b = data[offset++];
        if (i == 9 && (b & 0xfe)) return false; // Overflow beyond 64 bits.
        result |= uint64_t(b & 0x7f) << (7 * i);
        if (!(b & 0x80)) return true;
    }
    return false;
}

inline bool decode(const uint8_t *data, size_t size, std::vector<Field> &fields)
{
    fields.clear();
    size_t offset = 0;
    while (offset < size) {
        uint64_t tag = 0;
        if (!varint(data, size, offset, tag) || tag < 8 || (tag >> 3) > 0x1fffffff)
            return false;
        Field field{uint32_t(tag >> 3), uint32_t(tag & 7), 0};
        switch (field.wireType) {
        case 0:
            if (!varint(data, size, offset, field.value)) return false;
            break;
        case 1:
        case 5: {
            const size_t bytes = field.wireType == 1 ? 8 : 4;
            if (bytes > size - offset) return false;
            for (size_t i = 0; i < bytes; ++i)
                field.value |= uint64_t(data[offset++]) << (8 * i);
            break;
        }
        case 2: {
            uint64_t length = 0;
            if (!varint(data, size, offset, length) || length > size - offset)
                return false;
            offset += size_t(length);
            break;
        }
        default:
            return false;
        }
        fields.push_back(field);
    }
    return true;
}
} // namespace protobuf

#endif
