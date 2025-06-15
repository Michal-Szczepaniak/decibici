#ifndef SETTINGSSERVICE_H
#define SETTINGSSERVICE_H

#include <qble/qbleservice.h>

class SettingsService : public QBLEService
{
    Q_OBJECT
public:
    explicit SettingsService(const QString &uuid, const QString &path, QObject *parent = nullptr);

    static const char *UUID_SERVICE;
    static const char *UUID_DASHBOARD_CHARACTERISTIC;
    static const char *UUID_READ_CHARACTERISTIC;
    static const char *UUID_WRITE_CHARACTERISTIC;

    void readSpeed();
    void setLightsState(bool on);

private:
    QByteArray getChecksum(QByteArray in);
};

#endif // SETTINGSSERVICE_H
