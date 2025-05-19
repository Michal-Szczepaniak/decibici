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

    void setState(bool on);
};

#endif // ONOFFSERVICE_H
