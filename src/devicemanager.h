#ifndef DEVICEMANAGER_H
#define DEVICEMANAGER_H

#include "cowboydevice.h"

#include <QObject>

#include <qble/bluezadapter.h>

class DeviceManager : public QObject
{
    Q_OBJECT
    Q_PROPERTY(bool devicePaired READ getDevicePaired NOTIFY deviceConnectedChanged)
    Q_PROPERTY(bool deviceConnected READ getDeviceConnected NOTIFY deviceConnectedChanged)
public:
    explicit DeviceManager(QObject *parent = nullptr);

    bool getDevicePaired();
    bool getDeviceConnected();

    Q_INVOKABLE void setState(bool on);

signals:
    void deviceConnectedChanged();

private:
    BluezAdapter _adapter;
    CowboyDevice _device;

};

#endif // DEVICEMANAGER_H
