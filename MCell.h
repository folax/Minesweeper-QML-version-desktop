#ifndef CELL_H
#define CELL_H

enum cellState { eClosed, eQuestion, eFlag, eOpen, eDigit, eBomb };

class MCell
{

public:
    explicit MCell(int posX, int posY, bool visible = false);
    void setState(int state, bool bl) { m_state = state; m_cellIsOpened = bl; }
    void setStateDigit(int state, int digit, bool visible = false);
    void setFlagOnCell(const bool flag) { m_cellFlag = flag; }
    void setCellVisibility(const bool flag) { m_cellIsOpened = flag; }
    int getCellFlag() { return m_cellFlag; }
    int getCellState() { return m_state; }
    int getCellDigit() { return m_cellDigit; }
    bool getCellVisibility() { return m_cellIsOpened; }

    ~MCell();

public:
    int m_posX;
    int m_posY;
    int m_state;
    int m_cellDigit;
    bool m_cellFlag;

    bool m_cellIsOpened;
};

#endif // CELL_H


