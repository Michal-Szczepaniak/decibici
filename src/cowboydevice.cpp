#include "cowboydevice.h"
#include "onoffservice.h"
#include "settingsservice.h"
#include <QDomDocument>

CowboyDevice::CowboyDevice(QObject *parent) : QBLEDevice(parent)
{
    connect(this, &QBLEDevice::propertiesChanged, this, &CowboyDevice::onPropertiesChanged, Qt::UniqueConnection);
}

CowboyDevice::~CowboyDevice()
{
    QBLEDevice::disconnectFromDevice();
}

void CowboyDevice::connectToDevice()
{
    QBLEDevice::connectToDevice();
}

void CowboyDevice::disconnectFromDevice()
{
    QBLEDevice::disconnectFromDevice();
}

bool CowboyDevice::connected() const
{
    return _connected;
}

void CowboyDevice::onPropertiesChanged(const QString &interface, const QVariantMap &map, const QStringList &list)
{
    qDebug() << Q_FUNC_INFO << interface << map << list;

    if (interface == "org.bluez.Device1") {
        if (deviceProperty("ServicesResolved").toBool() ) {
            parseServices();
        }
        if (map.contains("Connected")) {
            _connected = map["Connected"].toBool();
            emit connectedChanged();
        }
    }
}

void CowboyDevice::parseServices()
{
    qDebug() << Q_FUNC_INFO;

    QDBusInterface adapterIntro("org.bluez", devicePath(), "org.freedesktop.DBus.Introspectable", QDBusConnection::systemBus(), 0);
    QDBusReply<QString> xml = adapterIntro.call("Introspect");

    qDebug() << "Resolved services...";

    qDebug().noquote() << xml.value();

    QDomDocument doc;
    doc.setContent(xml.value());

    QDomNodeList nodes = doc.elementsByTagName("node");

    qDebug() << nodes.count() << "nodes";

    for (int x = 0; x < nodes.count(); x++)
    {
        QDomElement node = nodes.at(x).toElement();
        QString nodeName = node.attribute("name");

        if (nodeName.startsWith("service")) {
            QString path = devicePath() + "/" + nodeName;

            QDBusInterface devInterface("org.bluez", path, "org.bluez.GattService1", QDBusConnection::systemBus(), 0);
            QString uuid = devInterface.property("UUID").toString();

            qDebug() << "Creating service for: " << uuid;

            if (uuid == OnOffService::UUID_SERVICE) {
                auto service = new OnOffService(uuid, path, this);
                connect(service, &OnOffService::stateChanged, this, &CowboyDevice::lockedChanged);
                connect(service, &OnOffService::lightsStateChanged, this, &CowboyDevice::lightsStateChanged);
                connect(service, &OnOffService::batteryChanged, this, &CowboyDevice::batteryChanged);
                connect(service, &OnOffService::batteryVoltageChanged, this, &CowboyDevice::batteryVoltageChanged);
                connect(service, &OnOffService::speedChanged, this, &CowboyDevice::speedChanged);
                connect(service, &OnOffService::assistanceChanged, this, &CowboyDevice::assistanceChanged);
                addService(uuid, service);
                service->readLockState();
            } else if (uuid == SettingsService::UUID_SERVICE) {
                auto service = new SettingsService(uuid, path, this);
                addService(uuid, service);
                service->readSpeed();
            } else {
                addService(uuid, new QBLEService(uuid, path, this));
            }
        }
    }

    emit servicesRegistered();
}
