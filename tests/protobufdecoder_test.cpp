#include "../src/protobufdecoder.h"
#include <cassert>
#include <cstdint>
#include <vector>

int main()
{
    std::vector<protobuf::Field> fields;
    // battery=85, lights=1, batteryTemp=25.5f, negative pedal torque (-2)
    const uint8_t packet[] = {
        0x30, 0x55, 0x40, 0x01,
        0x5d, 0x00, 0x00, 0xcc, 0x41,
        0x88, 0x01, 0xfe, 0xff, 0xff, 0xff, 0xff,
        0xff, 0xff, 0xff, 0xff, 0x01
    };
    assert(protobuf::decode(packet, sizeof(packet), fields));
    assert(fields.size() == 4);
    assert(fields[0].number == 6 && fields[0].value == 85);
    assert(fields[1].number == 8 && fields[1].value == 1);
    assert(fields[2].number == 11 && fields[2].wireType == 5
           && fields[2].value == 0x41cc0000);
    assert(fields[3].number == 17 && uint32_t(fields[3].value) == 0xfffffffe);

    const uint8_t truncated[] = {0x30, 0x80};
    assert(!protobuf::decode(truncated, sizeof(truncated), fields));
    const uint8_t badLength[] = {0x12, 0x03, 0xff};
    assert(!protobuf::decode(badLength, sizeof(badLength), fields));
    const uint8_t unknown[] = {0x9a, 0x06, 0x02, 0x00, 0x01};
    assert(protobuf::decode(unknown, sizeof(unknown), fields));
    assert(fields.size() == 1 && fields[0].number == 99);
}
