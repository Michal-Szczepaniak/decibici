#include "settingsservice.h"
#include <QDBusInterface>
#include <QDBusReply>
#include <QDebug>
#include <QDomDocument>

const char* SettingsService::UUID_SERVICE = "6e400001-b5a3-f393-e0a9-e50e24dcca9e";
const char* SettingsService::UUID_READ_CHARACTERISTIC = "6e400003-b5a3-f393-e0a9-e50e24dcca9e";
const char* SettingsService::UUID_WRITE_CHARACTERISTIC = "6e400002-b5a3-f393-e0a9-e50e24dcca9e";

SettingsService::SettingsService(const QString &uuid, const QString &path, QObject *parent) : QBLEService(uuid, path, parent)
{
    QDBusInterface introspection("org.bluez", path, "org.freedesktop.DBus.Introspectable",
                                QDBusConnection::systemBus());
    QDBusReply<QString> xml = introspection.call("Introspect");
    if (!xml.isValid()) {
        qWarning() << "Cannot inspect settings service:" << xml.error().message();
        return;
    }

    QDomDocument document;
    if (!document.setContent(xml.value())) {
        qWarning() << "Invalid settings service introspection XML";
        return;
    }
    const QDomNodeList nodes = document.elementsByTagName("node");
    for (int i = 0; i < nodes.count(); ++i) {
        const QString name = nodes.at(i).toElement().attribute("name");
        if (!name.startsWith("char")) continue;
        const QString characteristicPath = path + "/" + name;
        QDBusInterface characteristic("org.bluez", characteristicPath,
                                     "org.bluez.GattCharacteristic1", QDBusConnection::systemBus());
        if (characteristic.property("UUID").toString().compare(UUID_WRITE_CHARACTERISTIC,
                                                                 Qt::CaseInsensitive) == 0) {
            _writeCharacteristicPath = characteristicPath;
            break;
        }
    }
    if (_writeCharacteristicPath.isEmpty())
        qWarning() << "Settings write characteristic not found:" << UUID_WRITE_CHARACTERISTIC;
}

void SettingsService::readSpeed()
{
    QByteArray packet(7, 0);
    packet[0] = 10;
    packet[1] = 3;
    packet[2] = 0;
    packet[4] = 4;
    packet[5] = 0;
    packet[6] = 1;

    writeSettingsPacket(packet);
}

void SettingsService::setLightsState(bool on)
{
    QByteArray packet(11, 0);
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

    qInfo() << "Requesting lights" << (on ? "ON" : "OFF") << "packet" << packet.toHex();
    writeSettingsPacket(packet);
}

void SettingsService::writeSettingsPacket(const QByteArray &packet)
{
    if (_writeCharacteristicPath.isEmpty()) {
        qWarning() << "Cannot send settings packet: BLE write characteristic unavailable";
        return;
    }
    QDBusInterface characteristic("org.bluez", _writeCharacteristicPath,
                                 "org.bluez.GattCharacteristic1", QDBusConnection::systemBus());
    QDBusMessage reply = characteristic.call("WriteValue", packet, QVariantMap());
    if (reply.type() == QDBusMessage::ErrorMessage) {
        qWarning() << "Bluetooth WriteValue failed:" << reply.errorName() << reply.errorMessage();
    } else {
        qInfo() << "Bluetooth WriteValue accepted; awaiting bike state notification";
    }
}

QByteArray SettingsService::getChecksum(QByteArray in)
{
    quint16 crc = 0xffff;
    for (char byte : in) {
        crc ^= static_cast<quint8>(byte);
        for (int bit = 0; bit < 8; ++bit) {
            crc = (crc & 1) ? ((crc >> 1) ^ 0xa001) : (crc >> 1);
        }
    }
    QByteArray result(2, 0);
    result[0] = static_cast<char>(crc & 0xff);
    result[1] = static_cast<char>((crc >> 8) & 0xff);
    return result;
}
