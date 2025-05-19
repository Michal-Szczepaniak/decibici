import QtQuick 2.0
import Sailfish.Silica 1.0

CoverBackground {
    Column {
        anchors.centerIn: parent
        spacing: Theme.paddingLarge

        Label {
            id: label
            anchors.horizontalCenter: parent.horizontalCenter
            text: qsTr("decibici")
        }

        Label {
            id: status
            anchors.horizontalCenter: parent.horizontalCenter
            text: deviceManager.devicePaired ? (deviceManager.deviceConnected ? qsTr("Device connected") : qsTr("Device found")) : qsTr("Device not paired")
        }
    }

    CoverActionList {
        id: coverAction

        CoverAction {
            iconSource: "image://theme/icon-s-outline-secure"
            onTriggered: deviceManager.setState(false)
        }

        CoverAction {
            iconSource: "image://theme/icon-s-secure"
            onTriggered: deviceManager.setState(true)
        }
    }
}
