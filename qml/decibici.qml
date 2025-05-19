import QtQuick 2.0
import Sailfish.Silica 1.0
import com.verdanditeam.decibici 1.0
import "pages"

ApplicationWindow {
    id: app
    initialPage: Component { Main { } }
    cover: Qt.resolvedUrl("cover/CoverPage.qml")
    allowedOrientations: defaultAllowedOrientations
    property string version: "1.0"

    DeviceManager {
        id: deviceManager
    }
}
