import QtQuick 2.4

Item {
    property int cSize: 35
    property int cellIndex
    property bool leftMouseBtnPressed: false
    property bool rightMouseBtnPressed: false
    property bool rightMouseBtnHolded: false
    property bool leftMouseBtnHolded: false
    property bool leftRightMouseBtnUnHolded: false

    width: cSize
    height: cSize

    function setCellState(index)
    {
        var path
        if(cppObject.getCellVisibility(cellIndex))
        {
            if(cppObject.getCellState(cellIndex) == 3)
                path = "/images/images/cell-empty-icon.png"

            else if(cppObject.getCellState(cellIndex) == 4)
            {
                if(cppObject.getCellDigit(cellIndex) == 1)
                    path = "/images/images/hint-one-cell.png"
                else if(cppObject.getCellDigit(cellIndex) == 2)
                    path = "/images/images/hint-two-cell.png"
                else if(cppObject.getCellDigit(cellIndex) == 3)
                    path = "/images/images/hint-three-cell.png"
                else if(cppObject.getCellDigit(cellIndex) == 4)
                    path = "/images/images/hint-four-cell.png"
                else if(cppObject.getCellDigit(cellIndex) == 5)
                    path = "/images/images/hint-five-cell.png"
                else if(cppObject.getCellDigit(cellIndex) == 6)
                    path = "/images/images/hint-six-cell.png"
                else if(cppObject.getCellDigit(cellIndex) == 7)
                    path = "/images/images/hint-seven-cell.png"
                else if(cppObject.getCellDigit(cellIndex) == 8)
                    path = "/images/images/hint-eight-cell.png"
            }
            else
                path = "/images/images/cell-bomb-icon.png"
        }
        else if(cppObject.getCellState(cellIndex) == 2)
            path = "/images/images/cell-flag-icon.png"

        else
            path = "/images/images/cell-closed-icon.png"
        return path
    }

    Rectangle {
        id: rect
        width: cellSize
        height: cellSize
        border.width: 2
        border.color:  "#000066"

        Flipable {
            id: flipCell
            width: cSize
            height: cSize


            property bool flipped: false

            front: Image {
                id: frontImage
                source: "/images/images/cell-closed-icon.png";
                anchors.centerIn: parent
                width: rect.width - rect.border.width
                height: rect.height - rect.border.width
            }
            back: Image {
                id: backImage
                //source: "/images/images/hint-four-cell.png";
                anchors.centerIn: parent
                width: rect.width - rect.border.width
                height: rect.height - rect.border.width
            }

            transform: Rotation {
                id: rotation
                origin.x: flipCell.width/2
                origin.y: flipCell.height/2
                axis.x: 1; axis.y: 0; axis.z: 0
                angle: 0

            }

            states: State {
                name: "back"
                PropertyChanges { target: rotation; angle: 180 }
                when: flipCell.flipped
            }

            transitions: Transition {
                NumberAnimation { target: rotation; property: "angle"; duration: 550 }
            }

            Connections
            {
                target: cppObject;
                onImageUpdate:
                {
                    flipCell.flipped = true
                    backImage.source = setCellState(cellIndex)
                }
                onAreaResize:
                {
                    flipCell.flipped = false
                    frontImage.source = "/images/images/cell-closed-icon.png"
                }
            }

            MouseArea {
                //note: middleMouseClick second args: 1 = onMiddleButtonClick, 2 = onPressAndHold
                anchors.fill: parent
                acceptedButtons: Qt.LeftButton | Qt.RightButton | Qt.MiddleButton

                onClicked:
                {
                    if(mouse.button == Qt.LeftButton)
                    {
                        timer1.start();
                        cppObject.leftMouseBtnClick(cellIndex);
                        leftMouseBtnPressed = true;
                    }
                    if(mouse.button == Qt.RightButton)
                    {
                        cppObject.rightMouseBtnClick(cellIndex);
                        rightMouseBtnPressed = true;
                    }
                    if(mouse.button == Qt.MiddleButton)
                        cppObject.middleMouseClick(cellIndex, 1);

                    //both mouse buttons click handler;
                    if(rightMouseBtnPressed && leftMouseBtnPressed)
                        cppObject.middleMouseClick(cellIndex, 1);
                }

                onPressAndHold:
                {
                    if(mouse.button == Qt.MiddleButton)
                        cppObject.middleMouseClick(cellIndex , 2);
                    if(mouse.button == Qt.LeftButton)
                        leftMouseBtnHolded = true;
                    if(mouse.button == Qt.RightButton)
                        rightMouseBtnHolded = true;
                    if(rightMouseBtnHolded && leftMouseBtnHolded)
                    {
                        cppObject.middleMouseClick(cellIndex , 2);
                        leftRightMouseBtnUnHolded = true;
                    }
                }

                onReleased: {
                    if(mouse.button == Qt.MiddleButton)
                        cppObject.middleMouseClick(cellIndex , 3);
                    else if(mouse.button == Qt.RightButton)
                    {
                        rightMouseBtnPressed = false;
                        rightMouseBtnHolded = false;
                    }
                    else if(mouse.button == Qt.LeftButton)
                    {
                        leftMouseBtnPressed = false;
                        leftMouseBtnHolded = false;
                    }
                    if(leftRightMouseBtnUnHolded)
                    {
                        cppObject.middleMouseClick(cellIndex , 3);
                        leftRightMouseBtnUnHolded = false;
                    }

                }
            }
        }
    }
}
