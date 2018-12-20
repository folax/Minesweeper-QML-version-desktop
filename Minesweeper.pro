TEMPLATE = app

QT += widgets qml

CONFIG += c++11

win32 {
QMAKE_CXXFLAGS = -Wall -Wextra -Werror
QMAKE_CFLAGS = -Wall -Wextra -Werror
}

SOURCES += main.cpp \
    MinesArea.cpp \
    MCell.cpp

RESOURCES += qml.qrc

# Additional import path used to resolve QML modules in Qt Creator's code model
QML_IMPORT_PATH =

# Default rules for deployment.
include(deployment.pri)

HEADERS += \
    MinesArea.h \
    MCell.h

RC_ICONS += images/appico.ico
