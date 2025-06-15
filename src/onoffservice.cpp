#include "onoffservice.h"
#include <protopuf/message.h>

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

int OnOffService::getAssistance() const
{
    return _dashboard.assistance;
}

void OnOffService::onCharacteristicChanged(const QString &c, const QByteArray &value)
{
    if (c == UUID_UNLOCK_CHARACTERISTIC) {
        _locked = value[0] != 0x01;
        emit stateChanged(_locked);
    }

    if (c == UUID_DASHBOARD_CHARACTERISTIC) {
        using Dashboard = pp::message<
            pp::int32_field<"tripId", 1>,
            pp::int32_field<"duration", 2>,
            pp::int32_field<"speed", 3>,
            pp::int32_field<"power", 4>,
            pp::int32_field<"distance", 5>,
            pp::int32_field<"battery", 6>,
            pp::int32_field<"assistance", 7>,
            pp::int32_field<"lights", 8>,
            pp::int32_field<"batteryVoltage", 9>,
            pp::int32_field<"unknown10", 10>,
            pp::float_field<"batteryTemp", 11>,
            pp::int32_field<"unknown12", 12>,
            pp::int32_field<"unknown13", 13>,
            pp::int32_field<"unknown14", 14>,
            pp::int32_field<"activeDuration", 15>,
            pp::int32_field<"movingDuration", 16>,
            pp::int32_field<"pedalTorque", 17>
            >;

        std::vector<std::byte> data;
        for (char byte : value) {
            data.emplace_back((std::byte)byte);
        }
        using namespace pp;
        auto result = message_coder<Dashboard>::decode((bytes)data);
        const auto& [dashboard, _] = *result;

        if (dashboard.get<1>().has_value()) {
            _dashboard.tripId = dashboard.get<1>().value();
            qDebug() << "tripId: " << _dashboard.tripId;
        }

        if (dashboard.get<2>().has_value()) {
            _dashboard.duration = dashboard.get<2>().value();
            qDebug() << "duration: " << _dashboard.duration;
        }

        if (dashboard.get<3>().has_value()) {
            _dashboard.speed = dashboard.get<3>().value();
            qDebug() << "speed: " << _dashboard.speed;
            emit speedChanged(_dashboard.speed);
        }

        if (dashboard.get<4>().has_value()) {
            _dashboard.power = dashboard.get<4>().value();
            qDebug() << "power: " << _dashboard.power;
        }

        if (dashboard.get<5>().has_value()) {
            _dashboard.distance = dashboard.get<5>().value();
            qDebug() << "distance: " << _dashboard.distance;
        }

        if (dashboard.get<6>().has_value()) {
            _dashboard.battery = dashboard.get<6>().value();
            qDebug() << "battery: " << _dashboard.battery;
            emit batteryChanged(_dashboard.battery);
        }

        if (dashboard.get<7>().has_value()) {
            _dashboard.assistance = dashboard.get<7>().value();
            qDebug() << "assistance: " << _dashboard.assistance;
            emit assistanceChanged(_dashboard.assistance);
        }

        if (dashboard.get<8>().has_value()) {
            _dashboard.lights = dashboard.get<8>().value();
            qDebug() << "lights: " << _dashboard.lights;
            emit lightsStateChanged(_dashboard.lights);
        }

        if (dashboard.get<9>().has_value()) {
            _dashboard.batteryVoltage = dashboard.get<9>().value();
            qDebug() << "batteryVoltage: " << _dashboard.batteryVoltage;
            emit batteryVoltageChanged(_dashboard.batteryVoltage);
        }

        if (dashboard.get<10>().has_value()) {
            _dashboard.unknown10 = dashboard.get<10>().value();
            qDebug() << "unknown10: " << _dashboard.unknown10;
        }

        if (dashboard.get<11>().has_value()) {
            _dashboard.batteryTemp = dashboard.get<11>().value();
            qDebug() << "batteryTemp: " << _dashboard.batteryTemp;
        }

        if (dashboard.get<12>().has_value()) {
            _dashboard.unknown12 = dashboard.get<12>().value();
            qDebug() << "unknown12: " << _dashboard.unknown12;
        }

        if (dashboard.get<13>().has_value()) {
            _dashboard.unknown13 = dashboard.get<13>().value();
            qDebug() << "unknown13: " << _dashboard.unknown13;
        }

        if (dashboard.get<14>().has_value()) {
            _dashboard.unknown14 = dashboard.get<14>().value();
            qDebug() << "unknown14: " << _dashboard.unknown14;
        }

        if (dashboard.get<15>().has_value()) {
            _dashboard.activeDuration = dashboard.get<15>().value();
            qDebug() << "activeDuration: " << _dashboard.activeDuration;
        }

        if (dashboard.get<16>().has_value()) {
            _dashboard.movingDuration = dashboard.get<16>().value();
            qDebug() << "movingDuration: " << _dashboard.movingDuration;
        }

        if (dashboard.get<17>().has_value()) {
            _dashboard.pedalTorque = dashboard.get<17>().value();
            qDebug() << "pedalTorque: " << _dashboard.pedalTorque;
        }
    }
}
