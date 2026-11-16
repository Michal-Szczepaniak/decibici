#include "onoffservice.h"
#include "protobufdecoder.h"
#include <cstring>

const char* OnOffService::UUID_SERVICE = "c0b0a000-18eb-499d-b266-2f2910744274";
const char* OnOffService::UUID_UNLOCK_CHARACTERISTIC = "c0b0a001-18eb-499d-b266-2f2910744274";
const char* OnOffService::UUID_DASHBOARD_CHARACTERISTIC = "c0b0a00a-18eb-499d-b266-2f2910744274";

OnOffService::OnOffService(const QString &uuid, const QString &path, QObject *parent) : QBLEService(uuid, path, parent)
{
    enableNotification(UUID_UNLOCK_CHARACTERISTIC);
    enableNotification(UUID_DASHBOARD_CHARACTERISTIC);
    connect(this, &QBLEService::characteristicChanged, this, &OnOffService::onCharacteristicChanged);
}

bool OnOffService::getLockState()
{
    return _locked;
}

void OnOffService::readLockState()
{
    QByteArray packet = readValue(UUID_UNLOCK_CHARACTERISTIC);

    if (packet.isEmpty()) {
        qWarning() << "Cannot read bike lock state: empty BLE response";
        return;
    }

    _locked = packet[0] != 0x01;

    emit stateChanged(_locked);
}

void OnOffService::setLockState(bool state)
{
    QByteArray packet = {0};
    packet[0] = !state;

    writeValue(UUID_UNLOCK_CHARACTERISTIC, packet);
}

bool OnOffService::getLightsState() const
{
    return _dashboard.lights;
}

int OnOffService::getBattery() const
{
    return _dashboard.battery;
}

int OnOffService::getBatteryVoltage() const
{
    return _dashboard.batteryVoltage;
}

int OnOffService::getSpeed() const
{
    return _dashboard.speed;
}

int OnOffService::getDistance() const
{
    return _dashboard.distance;
}

int OnOffService::getPower() const
{
    return _dashboard.power;
}

void OnOffService::onCharacteristicChanged(const QString &c, const QByteArray &value)
{
    if (c == UUID_UNLOCK_CHARACTERISTIC) {
        if (value.isEmpty()) {
            qWarning() << "Ignoring empty lock state notification";
            return;
        }
        _locked = value[0] != 0x01;
        emit stateChanged(_locked);
    }

    if (c == UUID_DASHBOARD_CHARACTERISTIC) {
        std::vector<protobuf::Field> fields;
        if (!protobuf::decode(reinterpret_cast<const uint8_t *>(value.constData()),
                              size_t(value.size()), fields)) {
            qWarning() << "Ignoring malformed Cowboy dashboard notification:" << value.toHex();
            return;
        }

        for (const auto &field : fields) {
            if (field.number == 11 && field.wireType == 5) {
                const uint32_t bits = uint32_t(field.value);
                std::memcpy(&_dashboard.batteryTemp, &bits, sizeof(bits));
                qDebug() << "batteryTemp: " << _dashboard.batteryTemp;
                continue;
            }
            if (field.wireType != 0) continue;
            // int32 protobuf fields carry the low 32 bits of a varint.
            const uint32_t bits = uint32_t(field.value);
            int32_t number;
            std::memcpy(&number, &bits, sizeof(number));

            switch (field.number) {
        case 1:
            _dashboard.tripId = number;
            qDebug() << "tripId: " << _dashboard.tripId;
            break;
        case 2:
            _dashboard.duration = number;
            qDebug() << "duration: " << _dashboard.duration;
            break;
        case 3:
            _dashboard.speed = number;
            qDebug() << "speed: " << _dashboard.speed;
            emit speedChanged(_dashboard.speed);
            break;
        case 4:
            _dashboard.power = number;
            qDebug() << "power: " << _dashboard.power;
            emit powerChanged(_dashboard.power);
            break;
        case 5:
            _dashboard.distance = number;
            qDebug() << "distance: " << _dashboard.distance;
            emit distanceChanged(_dashboard.distance);
            break;
        case 6:
            _dashboard.battery = number;
            qDebug() << "battery: " << _dashboard.battery;
            emit batteryChanged(_dashboard.battery);
            break;
        case 7:
            _dashboard.assistance = number;
            qDebug() << "assistance: " << _dashboard.assistance;
            break;
        case 8:
            _dashboard.lights = number;
            qDebug() << "lights: " << _dashboard.lights;
            emit lightsStateChanged(_dashboard.lights);
            break;
        case 9:
            _dashboard.batteryVoltage = number;
            qDebug() << "batteryVoltage: " << _dashboard.batteryVoltage;
            emit batteryVoltageChanged(_dashboard.batteryVoltage);
            break;
        case 10:
            _dashboard.unknown10 = number;
            qDebug() << "unknown10: " << _dashboard.unknown10;
            break;
        case 12:
            _dashboard.unknown12 = number;
            qDebug() << "unknown12: " << _dashboard.unknown12;
            break;
        case 13:
            _dashboard.unknown13 = number;
            qDebug() << "unknown13: " << _dashboard.unknown13;
            break;
        case 14:
            _dashboard.unknown14 = number;
            qDebug() << "unknown14: " << _dashboard.unknown14;
            break;
        case 15:
            _dashboard.activeDuration = number;
            qDebug() << "activeDuration: " << _dashboard.activeDuration;
            break;
        case 16:
            _dashboard.movingDuration = number;
            qDebug() << "movingDuration: " << _dashboard.movingDuration;
            break;
        case 17:
            _dashboard.pedalTorque = number;
            qDebug() << "pedalTorque: " << _dashboard.pedalTorque;
            break;
        default:
            break;
            }
        }
    }
}
