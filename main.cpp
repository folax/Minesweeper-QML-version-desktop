#include <QApplication>
#include <QQmlApplicationEngine>
#include <QQmlEngine>
#include <QQmlContext>
#include <QDesktopWidget>
#include <QObject>
#include <QDebug>
#include "MinesArea.h"

int main(int argc, char *argv[])
{
    QApplication app(argc, argv);
    QDesktopWidget *desktop = QApplication::desktop();

    QScopedPointer<Minesweeper> minesweeper(new Minesweeper);
    QQmlApplicationEngine engine;
    engine.rootContext()->setContextProperty("cppObject", minesweeper.data());
    engine.load(QUrl(QStringLiteral("qrc:/main.qml")));

    //move window to center
    QObject *rootObject = engine.rootObjects().first();
    int x, y;
    x = (desktop->width() - rootObject->property("width").toInt()) / 2;
    y = (desktop->height() - rootObject->property("height").toInt()) / 2;
    rootObject->setProperty("x", x);
    rootObject->setProperty("y", y);

    return app.exec();
}
