#ifndef COWBOYDEVICE_H
#define COWBOYDEVICE_H

#include <qble/qbledevice.h>

class CowboyDevice : public QBLEDevice
{
    Q_OBJECT
public:
    explicit CowboyDevice(QObject *parent = nullptr);
    ~CowboyDevice();

    void pair() { QBLEDevice::pair(); };

    void connectToDevice();
    void disconnectFromDevice();

    bool connected() const;

signals:
    void connectedChanged();
    void servicesRegistered();
    void lockedChanged(bool locked);
    void lightsStateChanged(bool state);
    void batteryChanged(int battery);
    void batteryVoltageChanged(int batteryVoltage);
    void speedChanged(int speed);
    void assistanceChanged(int assistance);

public slots:
    void onPropertiesChanged(const QString &interface, const QVariantMap &map, const QStringList &list);

private:
    void parseServices();
    bool _connected = false;
};

#endif // COWBOYDEVICE_H
