#include "settingsservice.h"

const char* SettingsService::UUID_SERVICE = "6e400001-b5a3-f393-e0a9-e50e24dcca9e";
const char* SettingsService::UUID_READ_CHARACTERISTIC = "6e400003-b5a3-f393-e0a9-e50e24dcca9e";
const char* SettingsService::UUID_WRITE_CHARACTERISTIC = "6e400002-b5a3-f393-e0a9-e50e24dcca9e";

SettingsService::SettingsService(const QString &uuid, const QString &path, QObject *parent) : QBLEService(uuid, path, parent)
{
}

void SettingsService::readSpeed()
{
    QByteArray packet;
    packet[0] = 10;
    packet[1] = 3;
    packet[2] = 0;
    packet[4] = 4;
    packet[5] = 0;
    packet[6] = 1;

    writeValue(UUID_WRITE_CHARACTERISTIC, packet);
}

void SettingsService::setLightsState(bool on)
{
    QByteArray packet = {0};
    packet[0] = 10;
    packet[1] = 16;
    packet[2] = 0;
    packet[3] = 1;
    packet[4] = 0;
    packet[5] = 1;
    packet[6] = 2;
    packet[7] = 0;
    packet[8] = on;

    QByteArray checksum = getChecksum(packet);
    packet[9] = checksum[0];
    packet[10] = checksum[1];

    writeValue(UUID_WRITE_CHARACTERISTIC, packet);
}

QByteArray SettingsService::getChecksum(QByteArray in)
{
    int i = 0;
    int i2 = 65535;
    for (char b : in) {
        i2 ^= b & 255;
        for (int i3 = 8; i3 > 0; i3--) {
            int i4 = i2 & 1;
            i2 >>= 1;
            if (i4 != 0) {
                i2 ^= 40961;
            }
        }
    }
    QVector<int> iArr = {i2 & 255, (i2 >> 8) & 255};
    QByteArray arr;
    int i5 = 0;
    while (i < 2) {
        arr[i5] = (char) iArr[i];
        i++;
        i5++;
    }
    return arr;
}
