#ifndef MINESWEEPER_H
#define MINESWEEPER_H

#include <QObject>
#include <QVector>
#include <QSettings>
#include "MCell.h"

class Minesweeper : public QObject
{
    Q_OBJECT

public:
    explicit Minesweeper();

    Q_INVOKABLE int cellsWidthNumbers() { return m_cellsWidthSize; }
    Q_INVOKABLE int cellsHeightNumbers() { return m_cellsHeightSize; }
    Q_INVOKABLE void newGame();
    Q_INVOKABLE void setAreaSize(const int numbersX, const int numbersY, const int bombs = 0);
    Q_INVOKABLE int getCellState(const int);
    Q_INVOKABLE int getCellDigit(const int);
    Q_INVOKABLE bool getCellVisibility(const int);
    Q_INVOKABLE void leftMouseBtnClick(const int);
    Q_INVOKABLE void rightMouseBtnClick(const int);
    Q_INVOKABLE void middleMouseHold(const int);
    void firstAreaBuild(const int position);
    void foundEmptyCells(const int position);
    ~Minesweeper();

signals:
    void imageUpdate();
    void areaResize();
    void victorySignal();
    void loseSignal();


private:
    int m_cellsWidthSize;
    int m_cellsHeightSize;
    int m_bombsNumber;

    QVector<QVector <MCell*>> m_cells2dArr;

    //convertation position to X-Y coordinates
    QPair<int, int> posToCords(int position);
    int cordsToPos(const int, const int);

    QSettings m_settings;
    void readSettings();
    void writeSettings();

    //this function generate empty fields around start position;
    void removeElement(QVector<QPair <int, int> >&, int, int);
    void showAllBombs();
    void checkVictory();
};

#endif // MINESWEEPER_H
