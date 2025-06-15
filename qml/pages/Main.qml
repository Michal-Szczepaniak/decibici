import QtQuick 2.0
import Sailfish.Silica 1.0

Page {
    id: page

    allowedOrientations: Orientation.All

    SilicaFlickable {
        anchors.fill: parent

        PullDownMenu {
            MenuItem {
                text: qsTr("About")
                onClicked: {
                    pageStack.push(Qt.resolvedUrl("About.qml"))
                }
            }

            MenuItem {
                text: qsTr("Connect")
                onClicked: {
                    deviceManager.connectToDevice()
                }
            }
        }

        PageHeader {
            id: header
            title: deviceManager.devicePaired ? (deviceManager.deviceConnected ? (deviceManager.servicesRegistered ? qsTr("Device connected") : qsTr("Registering services")) : qsTr("Device found")) : qsTr("Device not paired")
        }

        TextSwitch {
            id: onButton

            anchors.horizontalCenter: parent.horizontalCenter
            anchors.top: header.bottom
            anchors.topMargin: Theme.paddingMedium
            checked: deviceManager.locked
            enabled: deviceManager.servicesRegistered

            text: deviceManager.locked ? qsTr("Locked") : qsTr("Unlocked")
            onClicked: deviceManager.locked = !deviceManager.locked
        }

        TextSwitch {
            id: lightsButton

            anchors.horizontalCenter: parent.horizontalCenter
            anchors.top: onButton.bottom
            anchors.topMargin: Theme.paddingMedium
            checked: deviceManager.lights
            enabled: deviceManager.servicesRegistered

            text: deviceManager.lights ? qsTr("Lights on") : qsTr("Lights off")
            onClicked: deviceManager.lights = !deviceManager.lights
        }

        Label {
            id: batteryLabel
            text: qsTr("Battery %1%").arg(deviceManager.battery)

            anchors.horizontalCenter: parent.horizontalCenter
            anchors.top: lightsButton.bottom
            anchors.topMargin: Theme.paddingMedium
        }

        Label {
            id: batteryVoltageLabel
            text: qsTr("Battery voltage %1V").arg(deviceManager.batteryVoltage/1000)

            anchors.horizontalCenter: parent.horizontalCenter
            anchors.top: batteryLabel.bottom
            anchors.topMargin: Theme.paddingMedium
        }

        Label {
            id: speedLabel
            text: qsTr("Speed %1km/h").arg(deviceManager.speed)

            anchors.horizontalCenter: parent.horizontalCenter
            anchors.top: batteryVoltageLabel.bottom
            anchors.topMargin: Theme.paddingMedium
        }

        Label {
            id: assistanceLabel
            text: qsTr("Assistance %1%").arg(deviceManager.assistance)

            anchors.horizontalCenter: parent.horizontalCenter
            anchors.top: speedLabel.bottom
            anchors.topMargin: Theme.paddingMedium
        }
    }
}
