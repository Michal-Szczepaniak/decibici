#ifndef COWBOYDEVICE_H
#define COWBOYDEVICE_H

#include <qble/qbledevice.h>
#define COWBOY_UART_WRITE_CHARACTERSTIC BT_UUID_DECLARE_128(BT_UUID_128_ENCODE(0X6E400002, 0XB5A3, 0XF393, 0XE0A9, 0XE50E24DCCA9E))

class CowboyDevice : public QBLEDevice
{
    Q_OBJECT
public:
    explicit CowboyDevice(QObject *parent = nullptr);

    void pair() { QBLEDevice::pair(); };

    void connectToDevice();
    void disconnectFromDevice();

    bool connected();

signals:
    void connectedChanged();

public slots:
    void onPropertiesChanged(const QString &interface, const QVariantMap &map, const QStringList &list);

private:
    void parseServices();
    bool _connected = false;
};

#endif // COWBOYDEVICE_H
