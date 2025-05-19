import QtQuick 2.0
import Sailfish.Silica 1.0

Page {
    id: page

    allowedOrientations: Orientation.All

    SilicaFlickable {
        anchors.fill: parent

        PageHeader {
            id: header
            title: deviceManager.devicePaired ? (deviceManager.deviceConnected ? qsTr("Device connected") : qsTr("Device found")) : qsTr("Device not paired")
        }

        Button {
            id: onButton

            anchors.horizontalCenter: parent.horizontalCenter
            anchors.top: header.bottom
            anchors.topMargin: Theme.paddingMedium

            text: qsTr("On")
            onClicked: deviceManager.setState(true);
        }

        Button {
            id: offButton

            anchors.horizontalCenter: parent.horizontalCenter
            anchors.top: onButton.bottom
            anchors.topMargin: Theme.paddingMedium

            text: qsTr("Off")
            onClicked: deviceManager.setState(false);
        }
    }
}
