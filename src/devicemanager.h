#ifndef DEVICEMANAGER_H
#define DEVICEMANAGER_H

#include "cowboydevice.h"

#include <QObject>

#include <qble/bluezadapter.h>

class DeviceManager : public QObject
{
    Q_OBJECT
    Q_PROPERTY(bool devicePaired READ getDevicePaired NOTIFY deviceConnectedChanged)
    Q_PROPERTY(bool servicesRegistered READ getServicesRegistered NOTIFY deviceConnectedChanged)
    Q_PROPERTY(bool deviceConnected READ getDeviceConnected NOTIFY deviceConnectedChanged)
    Q_PROPERTY(bool locked READ getLocked WRITE setLocked NOTIFY lockedChanged)
    Q_PROPERTY(bool lights READ getLights WRITE setLights NOTIFY lightsChanged)
    Q_PROPERTY(int battery READ getBattery NOTIFY batteryChanged)
    Q_PROPERTY(int batteryVoltage READ getBatteryVoltage NOTIFY batteryVoltageChanged)
    Q_PROPERTY(int speed READ getSpeed NOTIFY speedChanged)
    Q_PROPERTY(int assistance READ getAssistance NOTIFY assistanceChanged)
public:
    explicit DeviceManager(QObject *parent = nullptr);

    bool getDevicePaired();
    bool getServicesRegistered();
    bool getDeviceConnected();

    bool getLocked() const;
    void setLocked(bool locked);

    bool getLights() const;
    void setLights(bool lights);

    int getBattery() const;
    int getBatteryVoltage() const;
    int getSpeed() const;
    int getAssistance() const;

    Q_INVOKABLE void connectToDevice();

signals:
    void deviceConnectedChanged();
    void lockedChanged(bool locked);
    void lightsChanged(bool on);
    void batteryChanged(int battery);
    void batteryVoltageChanged(int batteryVoltage);
    void speedChanged(int speed);
    void assistanceChanged(int assistance);

private:
    BluezAdapter _adapter;
    CowboyDevice _device;
    bool _servicesRegistered = false;
};

#endif // DEVICEMANAGER_H
