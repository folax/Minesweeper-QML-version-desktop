import QtQuick 2.0
import QtQuick.Window 2.2
import QtQuick.Controls 1.4

ApplicationWindow {
    signal accept
    id: winOrLoseWindow
    visible: true

    property var mainText

    modality :  Qt.ApplicationModal

    // Window fit to center;
    x: (Screen.width - width) / 2
    y: (Screen.height - height) / 2

    minimumWidth: 400
    maximumWidth: 400
    minimumHeight: 200
    maximumHeight: 200
    title: "Minesweeper-result"

    Rectangle {
        id: rect
        width: 400
        height: 200
        color: "blue"

        Image {
            source: "/images/images/win-lose-background.png"
        }

        Text {
            id: mainLbl
            anchors.centerIn: parent
            color: "yellow"
            font.pixelSize: 48
            text: mainText
            style: Text.Outline; styleColor: "green"
        }

        Button {
            anchors.bottom: parent.bottom
            anchors.horizontalCenter: parent.horizontalCenter
            width: 100
            height: 33

            style: StyleForButton {}

            text: "Next"
            onClicked: {
                accept()
                winOrLoseWindow.close()
            }

        }
    }
    onClosing: {
        accept()
    }
}
