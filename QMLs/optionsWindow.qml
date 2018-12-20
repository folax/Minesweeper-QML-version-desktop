import QtQuick 2.0
import QtQuick.Window 2.2
import QtQuick.Controls 1.4
import QtQuick.Layouts 1.1
import QtQuick.Controls.Styles 1.4

Window {
    id: optionsRoot
    width: 370
    height: 360
    visible: true
    modality :  Qt.ApplicationModal
    maximumWidth: 370
    minimumWidth: 370
    maximumHeight: 360
    minimumHeight: 360

    // Window fit to center;
    x: (Screen.width - width) / 2
    y: (Screen.height - height) / 2

    property bool tFIsEnabled: false
    property int optionsWidth: 9
    property int optionsHeight: 9
    property int optionsBombs: 10

    signal activated(int optionsW, int optionsH, int optionsB)

    Image {
        width: root.width
        height: root.height
        source: "/images/images/options-background.png"
        fillMode: Image.Tile

        Row {
            spacing: 5
            GroupBox {
                title: "Options"

                ColumnLayout {
                    ExclusiveGroup { id: tabPositionGroup }

                    RadioButton {
                        text: "Beginner\n 10 Mines\n Area 9x9 cells"
                        checked: true
                        exclusiveGroup: tabPositionGroup
                        onClicked: {
                            tFIsEnabled = false
                            optionsWidth = 9
                            optionsHeight = 9
                            optionsBombs = 10
                        }
                    }

                    RadioButton {
                        text: "Amateur\n 40 Mines\n Area 16x16 cells"
                        exclusiveGroup: tabPositionGroup
                        onClicked: {
                            tFIsEnabled = false
                            optionsWidth = 16
                            optionsHeight = 16
                            optionsBombs = 40
                        }
                    }

                    RadioButton {
                        text: "Professional\n 99 Mines\n Area 16x30 cells"
                        exclusiveGroup: tabPositionGroup

                        onClicked: {
                            tFIsEnabled = false
                            optionsWidth = 30
                            optionsHeight = 16
                            optionsBombs = 99
                        }
                    }

                    RadioButton {
                        id: rbCustom
                        text: "Custom"
                        exclusiveGroup: tabPositionGroup
                        onClicked: {
                            tFIsEnabled = true
                            optionsWidth = tFWidth.text
                            optionsHeight = tFHeight.text
                            optionsBombs = tFBombs.text
                        }
                    }

                    Grid {
                        columns: 2
                        spacing: 2

                        Label {
                            text: "Width:  9 - 30"
                        }
                        TextField {
                            id: tFWidth
                            maximumLength: 2
                            width: 30
                            enabled: tFIsEnabled
                            validator: IntValidator {
                                bottom: 9
                                top: 30
                            }
                            text: qsTr("9")
                            onEditingFinished: {
                                if(parseInt(tFWidth.text * tFHeight.text) < parseInt(tFBombs.text))
                                    tFBombs.text = parseInt((tFHeight.text * tFWidth.text) * 0.3)
                            }
                        }

                        Label {
                            text: "Height: 9 - 24"
                        }
                        TextField {
                            id: tFHeight
                            maximumLength: 2
                            width: 30
                            enabled: tFIsEnabled
                            validator: IntValidator {
                                bottom: 9
                                top: 24
                            }
                            text: qsTr("9")
                            onEditingFinished: {
                                if(parseInt(tFWidth.text * tFHeight.text) < parseInt(tFBombs.text))
                                    tFBombs.text = parseInt((tFHeight.text * tFWidth.text) * 0.3)
                            }
                        }

                        Label {
                            text: "Mines:  10 - 500"
                        }
                        TextField {
                            id: tFBombs
                            maximumLength: 3
                            width: 30
                            enabled: tFIsEnabled
                            validator: IntValidator {
                                bottom: 10
                                top: 500
                            }
                            text: qsTr("10")
                            onEditingFinished: {
                                var bombs = parseInt(tFBombs.text)
                                if(bombs > parseInt((tFHeight.text * tFWidth.text) - parseInt(bombs * 0.39) ))
                                    tFBombs.text = parseInt((tFHeight.text * tFWidth.text) * 0.39)
                            }
                        }
                        Button {
                            text: "Ok"
                            onClicked: {
                                if(rbCustom.checked)
                                {
                                    optionsWidth = tFWidth.text
                                    optionsHeight = tFHeight.text
                                    optionsBombs = tFBombs.text
                                }
                                activated(optionsHeight, optionsWidth, optionsBombs)
                                optionsRoot.close()
                            }
                        }
                        Button {
                            text: "Cancel"
                            onClicked: {
                                optionsRoot.close()
                            }
                        }
                    }
                }
            }

            Image {
                id: optionsLogo
                x: 160
                width: 125
                height: 125
                fillMode: Image.PreserveAspectFit
                source: "/images/images/bomb-about.png"

                NumberAnimation on y {
                    to: 170
                    duration: 5000
                    onStopped: {
                        rotId.stop()
                    }
                }

                RotationAnimation on rotation {
                    id: rotId
                    from: 0
                    to: 360
                    duration: 5000
                    loops: Animation.Infinite
                }
            }
        }
    }
}
