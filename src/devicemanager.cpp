#include "devicemanager.h"
#include "onoffservice.h"
#include "settingsservice.h"

DeviceManager::DeviceManager(QObject *parent) : QObject(parent)
{
    connect(&_device, &CowboyDevice::connectedChanged, [&](){
        _servicesRegistered = false;
        emit deviceConnectedChanged();
    });
    connect(&_device, &CowboyDevice::servicesRegistered, [&](){
        _servicesRegistered = true;
        emit deviceConnectedChanged();
    });
    connect(&_device, &CowboyDevice::lockedChanged, this, &DeviceManager::lockedChanged);
    connect(&_device, &CowboyDevice::lightsStateChanged, this, &DeviceManager::lightsChanged);
    connect(&_device, &CowboyDevice::batteryChanged, this, &DeviceManager::batteryChanged);
    connect(&_device, &CowboyDevice::batteryVoltageChanged, this, &DeviceManager::batteryVoltageChanged);
    connect(&_device, &CowboyDevice::speedChanged, this, &DeviceManager::speedChanged);
    connect(&_device, &CowboyDevice::assistanceChanged, this, &DeviceManager::assistanceChanged);
}

bool DeviceManager::getDevicePaired()
{
    QString devicePath = _adapter.matchDevice("COWBOY");

    if (devicePath != "" && _device.devicePath() == "") {
        _device.setDevicePath(devicePath);
        _device.connectToDevice();
    }

    return devicePath != "";
}

bool DeviceManager::getServicesRegistered()
{
    return _servicesRegistered;
}

bool DeviceManager::getDeviceConnected()
{
    return _device.connected();
}

bool DeviceManager::getLocked() const
{
    if (!_device.connected()) return false;

    auto service = _device.service(OnOffService::UUID_SERVICE);

    if (service == nullptr) return false;

    return static_cast<OnOffService*>(service)->getLockState();
}

void DeviceManager::setLocked(bool locked)
{
    if (!_device.connected()) return;

    auto service = _device.service(OnOffService::UUID_SERVICE);

    if (service == nullptr) return;

    static_cast<OnOffService*>(service)->setLockState(locked);
}

bool DeviceManager::getLights() const
{
    if (!_device.connected()) return false;

    auto service = _device.service(OnOffService::UUID_SERVICE);

    if (service == nullptr) return false;

    return static_cast<OnOffService*>(service)->getLightsState();
}

void DeviceManager::setLights(bool lights)
{
    if (!_device.connected()) return;

    auto service = _device.service(SettingsService::UUID_SERVICE);

    if (service == nullptr) return;

    static_cast<SettingsService*>(service)->setLightsState(lights);
}

int DeviceManager::getBattery() const
{
    if (!_device.connected()) return false;

    auto service = _device.service(OnOffService::UUID_SERVICE);

    if (service == nullptr) return false;

    return static_cast<OnOffService*>(service)->getBattery();
}

int DeviceManager::getBatteryVoltage() const
{
    if (!_device.connected()) return false;

    auto service = _device.service(OnOffService::UUID_SERVICE);

    if (service == nullptr) return false;

    return static_cast<OnOffService*>(service)->getBatteryVoltage();
}

int DeviceManager::getSpeed() const
{
    if (!_device.connected()) return false;

    auto service = _device.service(OnOffService::UUID_SERVICE);

    if (service == nullptr) return false;

    return static_cast<OnOffService*>(service)->getSpeed();
}

int DeviceManager::getAssistance() const
{
    if (!_device.connected()) return false;

    auto service = _device.service(OnOffService::UUID_SERVICE);

    if (service == nullptr) return false;

    return static_cast<OnOffService*>(service)->getAssistance();
}

void DeviceManager::connectToDevice()
{
    QString devicePath = _adapter.matchDevice("COWBOY");

    if (devicePath != "" && _device.devicePath() == "") {
        _device.setDevicePath(devicePath);
        _device.connectToDevice();
    }
}
