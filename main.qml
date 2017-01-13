import QtQuick 2.4
import QtQuick.Controls 1.3
import QtQuick.Window 2.2
import QtQuick.Dialogs 1.2
import QtQuick.Layouts 1.1
import QtQuick.Controls.Styles 1.4
import "QMLs"

Window
{
    property int timerSeconds: 0
    property int cellSize: 35
    property bool firstStartFlag: true

    id: root
    visible: true

    //loader section;
    Loader {
        id: optionsWndw
        onLoaded: item.activated.connect(resizeAreaOptions)
    }

    Loader {
        id: winOrLoseWndw
        onLoaded: {
            item.accept.connect(newGame)
        }
    }

    function newGame()
    {
        cppObject.newGame();
        timerSeconds = 0;
        timer1.stop();
        timerText.text = "";
    }

    //get data options from options window;
    function resizeAreaOptions(optionsW, optionsH, optionsB)
    {
        cppObject.setAreaSize(optionsW, optionsH, optionsB)
        if(root.visibility != Window.Maximized)
            firstStartFlag = true
        setWindowSize()
    }

    function setWindowSize()
    {
        //If first start;
        if(firstStartFlag)
        {
            root.width = ((cellSize * cppObject.cellsHeightNumbers() + 60))
            root.height = ((cellSize * cppObject.cellsWidthNumbers() + 50))
            firstStartFlag = false
        }
    }

    Connections {
        target: cppObject
        onLoseSignal:
        {
            winOrLoseWndw.source = "/QMLs/resultWindow.qml"
            winOrLoseWndw.item.mainText = "You Lose!"
            winOrLoseWndw.item.show()
            timer1.stop()
        }
    }

    Connections {
        target: cppObject
        onVictorySignal:
        {
            winOrLoseWndw.source = "/QMLs/resultWindow.qml"
            winOrLoseWndw.item.mainText = "You Win!"
            winOrLoseWndw.item.show()
            timer1.stop()
        }
    }

    Connections {
        target: cppObject;
        onAreaResize:
        {
            cellsGrid.rows = cppObject.cellsWidthNumbers()
            cellsGrid.columns = cppObject.cellsHeightNumbers()
            repeat.model = cppObject.cellsWidthNumbers() * cppObject.cellsHeightNumbers()
        }
    }

    Rectangle
    {
        id: backgroundRect
        color: "black"
        width: Window.width
        height: Window.height

        Row
        {
            id: rowId
            anchors.horizontalCenter: parent.horizontalCenter
            spacing: 20
            y: 1

            Button
            {
                id: btnNewGame
                text: "New Game"
                width: root.width /4
                height: 33
                style: StyleForButton {}
                onClicked:
                {
                    newGame();
                }
            }
            Image
            {
                antialiasing: true
                width: 50
                height: 33
                fillMode: Image.PreserveAspectFit
                source: "/images/images/timer-background.png"
                Text {
                    id: timerText
                    color: "#00CCFF"
                    anchors.centerIn: parent
                    font.weight: Font.Bold
                    font.pointSize: 18
                    font.family: "Helvetica"
                    Timer {
                        id: timer1
                        interval: 1000
                        running: false
                        repeat: true
                        onTriggered:
                        {
                            timerText.text = timerSeconds
                            ++timerSeconds
                        }
                    }
                }
            }

            Button
            {
                id: btnOptions
                text: "Options"
                width: root.width /4
                height: 33
                style: StyleForButton {}
                onClicked: {
                    optionsWndw.source = "/QMLs/optionsWindow.qml"
                    optionsWndw.item.show()
                }
            }
        }
    }

    Rectangle
    {
        id: area
        width: root.width
        height: root.height - 38
        y: 35

        Image {
            width: root.width
            height: root.height
            source: "/images/images/app-background.png"
            fillMode: Image.Tile
        }

        Grid
        {
            id: cellsGrid
            rows: cppObject.cellsWidthNumbers()               //строка
            columns: cppObject.cellsHeightNumbers()           //столбец
            anchors.centerIn: parent

            Repeater
            {
                id: repeat
                model: cppObject.cellsWidthNumbers() * cppObject.cellsHeightNumbers()

                Cell
                {
                    cSize: cellSize
                    cellIndex: index
                }
            }
        }
    }
    Component.onCompleted: setWindowSize()
}

