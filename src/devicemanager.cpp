#include "devicemanager.h"
#include "onoffservice.h"

DeviceManager::DeviceManager(QObject *parent) : QObject(parent)
{
    connect(&_device, &CowboyDevice::connectedChanged, this, &DeviceManager::deviceConnectedChanged);
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

bool DeviceManager::getDeviceConnected()
{
    return _device.connected();
}

void DeviceManager::setState(bool state)
{
    if (!_device.connected()) return;

    auto service = _device.service(OnOffService::UUID_SERVICE);

    if (service == nullptr) return;

    static_cast<OnOffService*>(service)->setState(state);
}
