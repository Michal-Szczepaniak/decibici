#ifndef ONOFFSERVICE_H
#define ONOFFSERVICE_H

#include <qble/qbleservice.h>

class OnOffService : public QBLEService
{
    Q_OBJECT
public:
    OnOffService(const QString &uuid, const QString &path, QObject *parent = nullptr);

    static const char *UUID_SERVICE;
    static const char *UUID_UNLOCK_CHARACTERISTIC;
    static const char *UUID_DASHBOARD_CHARACTERISTIC;

    bool getLockState();
    void readLockState();
    void setLockState(bool on);

    bool getLightsState() const;
    int getBattery() const;
    int getBatteryVoltage() const;
    int getSpeed() const;
    int getAssistance() const;

signals:
    void stateChanged(bool state);
    void lightsStateChanged(bool state);
    void batteryChanged(int battery);
    void batteryVoltageChanged(int batteryVoltage);
    void speedChanged(int speed);
    void assistanceChanged(int assistance);

public slots:
    void onCharacteristicChanged(const QString &c, const QByteArray &value);

private:
    struct {
        int tripId;
        int duration;
        int speed;
        int power;
        int distance;
        int battery;
        int assistance;
        int lights;
        int batteryVoltage; // in mV
        int unknown10; // probably motor voltage or rpm (goes negative when accelerating)
        float batteryTemp; // in0.1 K
        int unknown12;
        int unknown13; // probably torque voltage or value (goes up when pedalling w/ motor cable disconnected)
        int unknown14;
        int activeDuration;
        int movingDuration;
        int pedalTorque;
    } _dashboard{};
    bool _locked = false;
};

#endif // ONOFFSERVICE_H
