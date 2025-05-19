#include "onoffservice.h"

const char* OnOffService::UUID_SERVICE = "c0b0a000-18eb-499d-b266-2f2910744274";
const char* OnOffService::UUID_UNLOCK_CHARACTERISTIC = "c0b0a001-18eb-499d-b266-2f2910744274";

OnOffService::OnOffService(const QString &uuid, const QString &path, QObject *parent) : QBLEService(uuid, path, parent)
{
}

void OnOffService::setState(bool state)
{
    QByteArray packet = {0};
    packet[0] = state;

    writeValue(UUID_UNLOCK_CHARACTERISTIC, packet);
}
