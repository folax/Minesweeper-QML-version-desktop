#include "MCell.h"

MCell::MCell(int posX, int posY, bool)
{
    m_posX = posX;
    m_posY = posY;
    m_state = eClosed;
    m_cellIsOpened = false;
    m_cellFlag = false;
}

void MCell::setStateDigit(int state, int digit, bool visible)
{
    if(state == eDigit)
    {
        m_state = state;
        m_cellDigit = digit;
        m_cellIsOpened = visible;
    }
}

MCell::~MCell()
{

}

