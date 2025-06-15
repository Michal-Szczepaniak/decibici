#include "devicemanager.h"

#include <QDomDocument>
#include <QtQuick>
#include <sailfishapp.h>

#include <qble/adaptermodel.h>
#include <qble/bluezadapter.h>
#include <protopuf/message.h>

int main(int argc, char *argv[])
{
    QScopedPointer<QGuiApplication> app(SailfishApp::application(argc, argv));
    QSharedPointer<QQuickView> view(SailfishApp::createView());

    qmlRegisterType<DeviceManager>("com.verdanditeam.decibici", 1, 0, "DeviceManager");

    view->setSource(SailfishApp::pathTo("qml/decibici.qml"));
    view->show();

    return app->exec();
}
