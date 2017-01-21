#include "MinesArea.h"
#include <QDebug>
#include <QVector>
#include <QPair>

Minesweeper::Minesweeper() : m_cellsWidthSize(9), m_cellsHeightSize(9), m_bombsNumber(10), m_settings("Decay", "Minesweeper")
{
    readSettings();
}

void Minesweeper::newGame()
{
    setAreaSize(m_cellsWidthSize, m_cellsHeightSize, m_bombsNumber);
}

void Minesweeper::setAreaSize(int numbersX, int numbersY, int bombs)
{
    m_bombsNumber = bombs;
    m_cells2dArr.clear();

    m_cellsWidthSize = numbersX;
    m_cellsHeightSize = numbersY;

    m_cells2dArr.resize(m_cellsWidthSize);
    for(int i(0); i < m_cellsWidthSize; ++i)
    {
        m_cells2dArr[i].resize(m_cellsHeightSize);
        for(int j(0); j < m_cellsHeightSize; ++j)
        {
            MCell *cell = new MCell(i, j);
            m_cells2dArr[i][j] = cell;
        }
    }
    emit areaResize();
}

int Minesweeper::getCellState(const int position)
{
    QPair<int, int> coords(posToCords(position));
    if(m_cells2dArr[coords.first][coords.second]->getCellFlag() == true)
    {
        return 2;
    }
    else
    {
        return m_cells2dArr[coords.first][coords.second]->getCellState();
    }
}

int Minesweeper::getCellDigit(const int position)
{
    QPair<int, int> coords(posToCords(position));
    return m_cells2dArr[coords.first][coords.second]->getCellDigit();
}

bool Minesweeper::getCellVisibility(const int position)
{
    QPair<int, int> coords(posToCords(position));
    return m_cells2dArr[coords.first][coords.second]->getCellVisibility();
}

void Minesweeper::leftMouseBtnClick(const int position)
{
    //проверка, если массив ищё не заполнен
    int counter = 0;
    for (QVector<MCell*> &vec_current : m_cells2dArr)
    {
        for (MCell* value : vec_current)
        {
            if(value->getCellState() == eClosed)
                ++counter;
        }
    }
    if(counter == (m_cells2dArr.size() * m_cells2dArr.at(0).size()))
    {
        firstAreaBuild(position);
    }

    //found empty cell and analize;
    foundEmptyCells(position);

    //check for victory;
    checkVictory();

    emit imageUpdate();
}

void Minesweeper::rightMouseBtnClick(const int position)
{
    QPair<int, int> coords(posToCords(position));
    int _posX = coords.first;
    int _posY = coords.second;

    if(m_cells2dArr[_posX][_posY]->getCellFlag() == true)
        m_cells2dArr[_posX][_posY]->setFlagOnCell(false);
    else
    {
        int counter = 0;
        for (QVector<MCell*> &vec_current : m_cells2dArr)
        {
            for (MCell* value : vec_current)
            {
                if(value->getCellState() == eClosed)
                    ++counter;
            }
        }
        if(counter != (m_cells2dArr.size() * m_cells2dArr.at(0).size()))
        {
            if(m_cells2dArr[_posX][_posY]->getCellFlag() == false &&
                    m_cells2dArr[_posX][_posY]->getCellVisibility() == false)
            {
                m_cells2dArr[_posX][_posY]->setFlagOnCell(true);
            }
        }
    }
    emit imageUpdate();
}

void Minesweeper::middleMouseHold(const int position)
{
    //note: coords first = y, coords second = x;
    QPair <int, int> coords(posToCords(position));
    QVector<QPair <int, int>> signsArr;
    int areaW = m_cells2dArr.size() - 1;
    int areaH = m_cells2dArr.at(0).size() - 1;

    //заполняем массив закрытыми клетками вокруг цифры;
    if(m_cells2dArr[coords.first][coords.second]->getCellState() == eDigit && m_cells2dArr[coords.first][coords.second]->getCellVisibility())
    {
        //вокруг клетки;
        if ((coords.first < areaH && coords.second < areaW) && (coords.first > 0 && coords.second > 0))
        {
            for(int i = coords.first - 1; i <= coords.first + 1; ++i)
            {
                for(int j = coords.second - 1; j <= coords.second + 1; ++j)
                {
                    if (m_cells2dArr[i][j]->getCellVisibility() == false)
                        signsArr.append(qMakePair(i, j));
                }
            }
        }
        //левый верхний угол;
        if (coords.first == 0 && coords.second == 0)
        {
            if (m_cells2dArr[coords.first + 1][coords.second]->getCellVisibility() == false)
                signsArr.append(qMakePair(coords.first + 1, coords.second));
            if (m_cells2dArr[coords.first][coords.second + 1]->getCellVisibility() == false)
                signsArr.append(qMakePair(coords.first, coords.second + 1));
            if (m_cells2dArr[coords.first + 1][coords.second + 1]->getCellVisibility() == false)
                signsArr.append(qMakePair(coords.first + 1, coords.second + 1));
        }
        //левый нижний угол;
        else if (coords.first == areaH && coords.second == 0)
        {
            if (m_cells2dArr[coords.first][coords.second + 1]->getCellVisibility() == false)
                signsArr.append(qMakePair(coords.first, coords.second + 1));
            if (m_cells2dArr[coords.first - 1][coords.second]->getCellVisibility() == false)
                signsArr.append(qMakePair(coords.first -1, coords.second));
            if (m_cells2dArr[coords.first - 1][coords.second + 1]->getCellVisibility() == false)
                signsArr.append(qMakePair(coords.first - 1, coords.second + 1));
        }
        //правый верхний угол;
        else if (coords.first == 0 && coords.second == areaW)
        {
            if (m_cells2dArr[coords.first][coords.second - 1]->getCellVisibility() == false)
                signsArr.append(qMakePair(coords.first, coords.second - 1));
            if (m_cells2dArr[coords.first + 1][coords.second]->getCellVisibility() == false)
                signsArr.append(qMakePair(coords.first + 1, coords.second));
            if (m_cells2dArr[coords.first + 1][coords.second - 1]->getCellVisibility() == false)
                signsArr.append(qMakePair(coords.first + 1, coords.second - 1));
        }
        //правый нижний угол;
        else if (coords.first == areaH && coords.second == areaW)
        {
            if (m_cells2dArr[coords.first][coords.second - 1]->getCellVisibility() == false)
                signsArr.append(qMakePair(coords.first, coords.second - 1));
            if (m_cells2dArr[coords.first - 1][coords.second]->getCellVisibility() == false)
                signsArr.append(qMakePair(coords.first - 1, coords.second));
            if (m_cells2dArr[coords.first - 1][coords.second - 1]->getCellVisibility() == false)
                signsArr.append(qMakePair(coords.first - 1, coords.second - 1));
        }

        /* вторая часть проверки; */

        //проверка слева;
        else if (coords.second == 0)
        {
            if (m_cells2dArr[coords.first][coords.second + 1]->getCellVisibility() == false)
                signsArr.append(qMakePair(coords.first, coords.second + 1));
            if (m_cells2dArr[coords.first + 1][coords.second]->getCellVisibility() == false)
                signsArr.append(qMakePair(coords.first + 1, coords.second));
            if (m_cells2dArr[coords.first + 1][coords.second + 1]->getCellVisibility() == false)
                signsArr.append(qMakePair(coords.first + 1, coords.second + 1));
            if (m_cells2dArr[coords.first - 1][coords.second]->getCellVisibility() == false)
                signsArr.append(qMakePair(coords.first - 1, coords.second));
            if (m_cells2dArr[coords.first - 1][coords.second + 1]->getCellVisibility() == false)
                signsArr.append(qMakePair(coords.first - 1, coords.second + 1));
        }
        //проверка справа;
        else if (coords.second == areaW)
        {
            if (m_cells2dArr[coords.first][coords.second - 1]->getCellVisibility() == false)
                signsArr.append(qMakePair(coords.first, coords.second - 1));
            if (m_cells2dArr[coords.first + 1][coords.second - 1]->getCellVisibility() == false)
                signsArr.append(qMakePair(coords.first + 1, coords.second - 1));
            if (m_cells2dArr[coords.first + 1][coords.second]->getCellVisibility() == false)
                signsArr.append(qMakePair(coords.first + 1, coords.second));
            if (m_cells2dArr[coords.first - 1][coords.second - 1]->getCellVisibility() == false)
                signsArr.append(qMakePair(coords.first - 1, coords.second - 1));
            if (m_cells2dArr[coords.first - 1][coords.second]->getCellVisibility() == false)
                signsArr.append(qMakePair(coords.first - 1, coords.second));
        }
        //проверка сверху;
        else if (coords.first == 0)
        {
            if (m_cells2dArr[coords.first][coords.second - 1]->getCellVisibility() == false)
                signsArr.append(qMakePair(coords.first, coords.second - 1));
            if (m_cells2dArr[coords.first][coords.second + 1]->getCellVisibility() == false)
                signsArr.append(qMakePair(coords.first, coords.second + 1));
            if (m_cells2dArr[coords.first + 1][coords.second - 1]->getCellVisibility() == false)
                signsArr.append(qMakePair(coords.first + 1, coords.second - 1));
            if (m_cells2dArr[coords.first + 1][coords.second]->getCellVisibility() == false)
                signsArr.append(qMakePair(coords.first + 1, coords.second));
            if (m_cells2dArr[coords.first + 1][coords.second + 1]->getCellVisibility() == false)
                signsArr.append(qMakePair(coords.first + 1, coords.second + 1));
        }
        //проверка снизу;
        else if (coords.first == areaH)
        {
            if (m_cells2dArr[coords.first][coords.second - 1]->getCellVisibility() == false)
                signsArr.append(qMakePair(coords.first, coords.second - 1));
            if (m_cells2dArr[coords.first][coords.second + 1]->getCellVisibility() == false)
                signsArr.append(qMakePair(coords.first, coords.second + 1));
            if (m_cells2dArr[coords.first - 1][coords.second - 1]->getCellVisibility() == false)
                signsArr.append(qMakePair(coords.first - 1, coords.second - 1));
            if (m_cells2dArr[coords.first - 1][coords.second]->getCellVisibility() == false)
                signsArr.append(qMakePair(coords.first - 1, coords.second));
            if (m_cells2dArr[coords.first - 1][coords.second + 1]->getCellVisibility() == false)
                signsArr.append(qMakePair(coords.first - 1, coords.second + 1));
        }

        int cnt = 0;
        for(int i(0); i < signsArr.size(); ++i)
        {
            if(m_cells2dArr[signsArr.at(i).first][signsArr.at(i).second]->getCellFlag() == true)
            {
                cnt++;
            }
        }
        int number = m_cells2dArr[coords.first][coords.second]->getCellDigit();
        bool hasBomb = false;
        //если количество флагов равно значению цифры в нажатйо клетке то;
        if(cnt == number)
        {
            for(int i(0); i < signsArr.size(); ++i)
            {
                //если не значение клетки не равно флагу то;
                if(!m_cells2dArr[signsArr.at(i).first][signsArr.at(i).second]->getCellFlag())
                {
                    m_cells2dArr[signsArr.at(i).first][signsArr.at(i).second]->setCellVisibility(true);

                    if(m_cells2dArr[signsArr.at(i).first][signsArr.at(i).second]->getCellState() == eBomb)
                        hasBomb = true;
                    if(m_cells2dArr[signsArr.at(i).first][signsArr.at(i).second]->getCellState() == eClosed)
                    {
                        foundEmptyCells(cordsToPos(signsArr.at(i).first, signsArr.at(i).second));
                    }
                }
            }
        }
        if(hasBomb)
            showAllBombs();
    }
    checkVictory();
    emit imageUpdate();
}

void Minesweeper::firstAreaBuild(int position)
{
    //INFO: cells areas sizes 81, 256, 480 cells : 10, 40, 99 bombs : cells2dArr.size() = ширина, cells2dArr.at(0).size() = высота

    QPair<int, int> coords(posToCords(position));
    //qDebug() << "First: " << coords.first << " Second: " << coords.second;

    //переменные для работы;
    int _cellsSize = m_cells2dArr.size() * m_cells2dArr.at(0).size();
    //int _randX = 0;
    //int _randY = 0;
    int bombs = 0;
    int areaW = m_cells2dArr.size() - 1;
    int areaH = m_cells2dArr.at(0).size() - 1;
    //в зависимости от размера поля устанавливаем количество бомб;
    if (_cellsSize == 81)
        bombs = 10;
    else if (_cellsSize == 256)
        bombs = 40;
    else if (_cellsSize == 480)
        bombs = 99;
    if(m_bombsNumber != 0)
    {
        bombs = m_bombsNumber;
    }
    m_bombsNumber = bombs;

    //расставляем бомбы на поле;
    int i = 0;
    bool emptyNeighborhood = true;
    //*********************************
    QVector<QPair <int, int> > cordsPair;
    for(int i(0); i <= m_cells2dArr.size() - 1; ++i)
    {
        for(int j(0); j <= m_cells2dArr.at(0).size() - 1; ++j)
            cordsPair.append(qMakePair(i, j));
    }
    int pos;
    while(i < bombs)
    {
        pos = rand() % cordsPair.size();
        if(m_cells2dArr[cordsPair.at(pos).first][cordsPair.at(pos).second]->getCellState() == eClosed
                && (coords != cordsPair.at(pos)))
        {
            m_cells2dArr[cordsPair.at(pos).first][cordsPair.at(pos).second]->setState(eBomb, false);
            //qDebug() << "Removed: " <<  cordsPair.at(pos);
            cordsPair.remove(pos);

            //создаём пустые клетки вокруг стартовой клетки;
            if(emptyNeighborhood)
            {
                //angles;
                if(coords.first == 0 && coords.second == 0)
                {
                    //qDebug() << "Left Top Angle";
                    removeElement(cordsPair, coords.first, coords.second + 1);
                    removeElement(cordsPair, coords.first + 1, coords.second);
                    removeElement(cordsPair, coords.first + 1, coords.second + 1);
                }
                else if(coords.first == areaW && coords.second == 0)
                {
                    //qDebug() << "Left Bottom Angle";
                    removeElement(cordsPair, coords.first, coords.second + 1);
                    removeElement(cordsPair, coords.first - 1, coords.second);
                    removeElement(cordsPair, coords.first - 1, coords.second + 1);
                }
                else if(coords.first == areaW && coords.second == areaH)
                {
                    //qDebug() << "Right Bottom Angle";
                    removeElement(cordsPair, coords.first, coords.second - 1);
                    removeElement(cordsPair, coords.first - 1, coords.second);
                    removeElement(cordsPair, coords.first - 1, coords.second - 1);
                }
                else if(coords.first == 0 && coords.second == areaH)
                {
                    //qDebug() << "Right Top Angle";
                    removeElement(cordsPair, coords.first, coords.second - 1);
                    removeElement(cordsPair, coords.first + 1, coords.second - 1);
                    removeElement(cordsPair, coords.first + 1, coords.second);
                }
                //parallels;
                else if(coords.second == 0 && coords.first > 0 && coords.first < areaW)
                {
                    //qDebug() << "Left Down";
                    removeElement(cordsPair, coords.first, coords.second + 1);
                    removeElement(cordsPair, coords.first + 1, coords.second);
                    removeElement(cordsPair, coords.first + 1, coords.second + 1);
                    removeElement(cordsPair, coords.first - 1, coords.second);
                    removeElement(cordsPair, coords.first - 1, coords.second + 1);
                }
                else if(coords.second == areaH && coords.first > 0 && coords.first < areaW)
                {
                    //qDebug() << "Right Down";
                    removeElement(cordsPair, coords.first, coords.second - 1);
                    removeElement(cordsPair, coords.first + 1, coords.second);
                    removeElement(cordsPair, coords.first + 1, coords.second - 1);
                    removeElement(cordsPair, coords.first - 1, coords.second);
                    removeElement(cordsPair, coords.first - 1, coords.second - 1);
                }
                else if(coords.first == 0 && coords.second > 0 && coords.second < areaH)
                {
                    //qDebug() << "Upper Straight";
                    removeElement(cordsPair, coords.first, coords.second + 1);
                    removeElement(cordsPair, coords.first, coords.second - 1);
                    removeElement(cordsPair, coords.first + 1, coords.second - 1);
                    removeElement(cordsPair, coords.first + 1, coords.second);
                    removeElement(cordsPair, coords.first + 1, coords.second + 1);

                }
                else if(coords.first == areaW && coords.second > 0 && coords.second < areaH)
                {
                    //qDebug() << "Bottom Straight";
                    removeElement(cordsPair, coords.first, coords.second + 1);
                    removeElement(cordsPair, coords.first, coords.second - 1);
                    removeElement(cordsPair, coords.first - 1, coords.second - 1);
                    removeElement(cordsPair, coords.first - 1, coords.second);
                    removeElement(cordsPair, coords.first - 1, coords.second + 1);
                }
                else
                {
                    //qDebug() << "Middle area";
                    removeElement(cordsPair, coords.first, coords.second - 1);
                    removeElement(cordsPair, coords.first, coords.second + 1);
                    removeElement(cordsPair, coords.first - 1, coords.second - 1);
                    removeElement(cordsPair, coords.first - 1, coords.second);
                    removeElement(cordsPair, coords.first - 1, coords.second + 1);
                    removeElement(cordsPair, coords.first + 1, coords.second - 1);
                    removeElement(cordsPair, coords.first + 1, coords.second);
                    removeElement(cordsPair, coords.first + 1, coords.second + 1);

                }
                emptyNeighborhood = false;
            }
            ++i;
        }
    }

    //сколько бомб вокруг клетки;
    int _sign = 0, i2, j2;

    for(int i(0); i <= areaW; ++i)
    {
        for(int j(0); j <= areaH; ++j)
        {
            _sign = 0;
            if (m_cells2dArr[i][j]->getCellState() == eClosed)
            {
                //проверка соседних клеток, на расстоянии от границ в 1;
                if (((i > 0) && (j > 0)) && ((i < areaW) && ( j < areaH)))
                {
                    //проверяем 3 элемента;
                    for(i2 = i - 1; i2 <= i + 1; ++i2)
                    {
                        for(j2 = j - 1; j2 <= j + 1; ++j2)
                        {
                            if (m_cells2dArr[i2][j2]->getCellState() == eBomb)
                            {
                                ++_sign;
                            }
                        }
                    }
                }
                //проверка левого верхнего угла
                else if ((i == 0) && (j == 0))
                {
                    if(m_cells2dArr[1][0]->getCellState() == eBomb)
                        ++_sign;
                    if(m_cells2dArr[1][1]->getCellState() == eBomb)
                        ++_sign;
                    if(m_cells2dArr[0][1]->getCellState() == eBomb)
                        ++_sign;
                }
                //проверка левого нижнего угла
                else if ((i == 0) && (j == areaH))
                {
                    if(m_cells2dArr[0][areaH - 1]->getCellState() == eBomb)
                        ++_sign;
                    if(m_cells2dArr[1][areaH - 1]->getCellState() == eBomb)
                        ++_sign;
                    if(m_cells2dArr[1][areaH]->getCellState() == eBomb)
                        ++_sign;
                }
                //проверка правого нижнего угла
                else if ((i == areaW) && (j == areaH))
                {
                    if(m_cells2dArr[areaW][areaH - 1]->getCellState() == eBomb)
                        ++_sign;
                    if(m_cells2dArr[areaW - 1][areaH - 1]->getCellState() == eBomb)
                        ++_sign;
                    if(m_cells2dArr[areaW - 1][areaH]->getCellState() == eBomb)
                        ++_sign;
                }
                //проверка правого верхнего угла
                else if ((i == areaW) && (j == 0))
                {
                    if(m_cells2dArr[areaW - 1][0]->getCellState() == eBomb)
                        ++_sign;
                    if(m_cells2dArr[areaW - 1][1]->getCellState() == eBomb)
                        ++_sign;
                    if(m_cells2dArr[areaW][1]->getCellState() == eBomb)
                        ++_sign;
                }
                //проверка клеток с левого края.
                else
                {
                    if (i == 0)
                    {
                        if(m_cells2dArr[0][j - 1]->getCellState() == eBomb)
                            ++_sign;
                        if(m_cells2dArr[0][j + 1]->getCellState() == eBomb)
                            ++_sign;
                        if(m_cells2dArr[1][j - 1]->getCellState() == eBomb)
                            ++_sign;
                        if(m_cells2dArr[1][j]->getCellState() == eBomb)
                            ++_sign;
                        if(m_cells2dArr[1][j + 1]->getCellState() == eBomb)
                            ++_sign;
                    }
                    //проверка клеток с правого края
                    else if (i == areaW)
                    {
                        if(m_cells2dArr[areaW][j - 1]->getCellState() == eBomb)
                            ++_sign;
                        if(m_cells2dArr[areaW][j + 1]->getCellState() == eBomb)
                            ++_sign;
                        if(m_cells2dArr[areaW - 1][j - 1]->getCellState() == eBomb)
                            ++_sign;
                        if(m_cells2dArr[areaW - 1][j]->getCellState() == eBomb)
                            ++_sign;
                        if(m_cells2dArr[areaW - 1][j + 1]->getCellState() == eBomb)
                            ++_sign;
                    }
                    //проверка клеток с верхнего края
                    else if (j == 0)
                    {
                        if(m_cells2dArr[i - 1][0]->getCellState() == eBomb)
                            ++_sign;
                        if(m_cells2dArr[i + 1][0]->getCellState() == eBomb)
                            ++_sign;
                        if(m_cells2dArr[i - 1][1]->getCellState() == eBomb)
                            ++_sign;
                        if(m_cells2dArr[i][1]->getCellState() == eBomb)
                            ++_sign;
                        if(m_cells2dArr[i + 1][1]->getCellState() == eBomb)
                            ++_sign;
                    }
                    //проверка от нижней границы между крайними точками
                    else
                    {
                        if (j == areaH)
                        {
                            if(m_cells2dArr[i - 1][areaH]->getCellState() == eBomb)
                                ++_sign;
                            if(m_cells2dArr[i + 1][areaH]->getCellState() == eBomb)
                                ++_sign;
                            if(m_cells2dArr[i - 1][areaH - 1]->getCellState() == eBomb)
                                ++_sign;
                            if(m_cells2dArr[i][areaH - 1]->getCellState() == eBomb)
                                ++_sign;
                            if(m_cells2dArr[i + 1][areaH - 1]->getCellState() == eBomb)
                                ++_sign;
                        }
                    }
                }
            }
            if (_sign > 0)
                m_cells2dArr[i][j]->setStateDigit(eDigit, _sign, false);
        }
    }
}

void Minesweeper::foundEmptyCells(int position)
{
    QPair<int, int> coords(posToCords(position));
    int _posX = coords.first;
    int _posY = coords.second;

    QVector<QPair <int, int>> emptyCellsAnalizeVec;

    //если цифра то открываем её;
    if (m_cells2dArr[_posX][_posY]->getCellState() == eDigit)
        m_cells2dArr[_posX][_posY]->setState(eDigit, true);

    if (m_cells2dArr[_posX][_posY]->getCellState() == eClosed)
    {
        int areaW = m_cells2dArr.size() - 1;
        int areaH = m_cells2dArr.at(0).size() - 1;

        if ( (_posX < areaW && _posY < areaH) && (_posX > 0 && _posY > 0) )
        {
            for(int i = _posX - 1; i <= _posX + 1; ++i)
            {
                for(int j = _posY - 1; j <= _posY + 1; ++j)
                {
                    if (m_cells2dArr[i][j]->getCellState() == eClosed)
                        emptyCellsAnalizeVec.append(qMakePair(i, j));
                    if (m_cells2dArr[i][j]->getCellState() == eDigit)
                        m_cells2dArr[i][j]->setState(eDigit, true);
                }
            }
        }

        //левый верхний угол;
        else if (_posX == 0 && _posY == 0)
        {
            if (m_cells2dArr[_posX + 1][_posY]->getCellState() == eClosed)
                emptyCellsAnalizeVec.append(qMakePair(_posX + 1, _posY));
            if (m_cells2dArr[_posX + 1][_posY]->getCellState() == eDigit)
                m_cells2dArr[_posX + 1][_posY]->setState(eDigit, true);
            if (m_cells2dArr[_posX][_posY + 1]->getCellState() == eClosed)
                emptyCellsAnalizeVec.append(qMakePair(_posX, _posY + 1));
            if (m_cells2dArr[_posX][_posY + 1]->getCellState() == eDigit)
                m_cells2dArr[_posX][_posY + 1]->setState(eDigit, true);
            if (m_cells2dArr[_posX + 1][_posY + 1]->getCellState() == eClosed)
                emptyCellsAnalizeVec.append(qMakePair(_posX + 1, _posY + 1));
            if (m_cells2dArr[_posX + 1][_posY + 1]->getCellState() == eDigit)
                m_cells2dArr[_posX + 1][_posY + 1]->setState(eDigit, true);
        }
        //левый нижний угол;
        else if (_posX == 0 && _posY == areaH)
        {
            if (m_cells2dArr[_posX][_posY - 1]->getCellState() == eClosed)
                emptyCellsAnalizeVec.append(qMakePair(_posX, _posY - 1));
            if (m_cells2dArr[_posX][_posY - 1]->getCellState() == eDigit)
                m_cells2dArr[_posX][_posY - 1]->setState(eDigit, true);
            if (m_cells2dArr[_posX + 1][_posY - 1]->getCellState() == eClosed)
                emptyCellsAnalizeVec.append(qMakePair(_posX + 1, _posY - 1));
            if (m_cells2dArr[_posX + 1][_posY - 1]->getCellState() == eDigit)
                m_cells2dArr[_posX + 1][_posY - 1]->setState(eDigit, true);
            if (m_cells2dArr[_posX + 1][_posY]->getCellState() == eClosed)
                emptyCellsAnalizeVec.append(qMakePair(_posX + 1, _posY));
            if (m_cells2dArr[_posX + 1][_posY]->getCellState() == eDigit)
                m_cells2dArr[_posX + 1][_posY]->setState(eDigit, true);
        }
        //правый верхний угол;
        else if (_posX == areaW && _posY == 0)
        {
            if (m_cells2dArr[_posX - 1][_posY]->getCellState() == eClosed)
                emptyCellsAnalizeVec.append(qMakePair(_posX - 1, _posY));
            if (m_cells2dArr[_posX - 1][_posY]->getCellState() == eDigit)
                m_cells2dArr[_posX - 1][_posY]->setState(eDigit, true);
            if (m_cells2dArr[_posX - 1][_posY + 1]->getCellState() == eClosed)
                emptyCellsAnalizeVec.append(qMakePair(_posX - 1, _posY + 1));
            if (m_cells2dArr[_posX - 1][_posY + 1]->getCellState() == eDigit)
                m_cells2dArr[_posX - 1][_posY + 1]->setState(eDigit, true);
            if (m_cells2dArr[_posX][_posY + 1]->getCellState() == eClosed)
                emptyCellsAnalizeVec.append(qMakePair(_posX, _posY + 1));
            if (m_cells2dArr[_posX][_posY + 1]->getCellState() == eDigit)
                m_cells2dArr[_posX][_posY + 1]->setState(eDigit, true);
        }
        //правый нижний угол;
        else if (_posX == areaW && _posY == areaH)
        {
            if (m_cells2dArr[_posX - 1][_posY - 1]->getCellState() == eClosed)
                emptyCellsAnalizeVec.append(qMakePair(_posX - 1, _posY - 1));
            if (m_cells2dArr[_posX - 1][_posY - 1]->getCellState() == eDigit)
                m_cells2dArr[_posX - 1][_posY - 1]->setState(eDigit, true);
            if (m_cells2dArr[_posX][_posY - 1]->getCellState() == eClosed)
                emptyCellsAnalizeVec.append(qMakePair(_posX, _posY - 1));
            if (m_cells2dArr[_posX][_posY - 1]->getCellState() == eDigit)
                m_cells2dArr[_posX][_posY - 1]->setState(eDigit, true);
            if (m_cells2dArr[_posX - 1][_posY]->getCellState() == eClosed)
                emptyCellsAnalizeVec.append(qMakePair(_posX - 1, _posY));
            if (m_cells2dArr[_posX - 1][_posY]->getCellState() == eDigit)
                m_cells2dArr[_posX - 1][_posY]->setState(eDigit, true);
        }

        /* вторая часть проверки; */

        //проверка слева;
        else if (_posX == 0)
        {
            if (m_cells2dArr[_posX][_posY - 1]->getCellState() == eClosed)
                emptyCellsAnalizeVec.append(qMakePair(_posX, _posY - 1));
            if (m_cells2dArr[_posX][_posY - 1]->getCellState() == eDigit)
                m_cells2dArr[_posX][_posY - 1]->setState(eDigit, true);
            if (m_cells2dArr[_posX][_posY + 1]->getCellState() == eClosed)
                emptyCellsAnalizeVec.append(qMakePair(_posX, _posY + 1));
            if (m_cells2dArr[_posX][_posY + 1]->getCellState() == eDigit)
                m_cells2dArr[_posX][_posY + 1]->setState(eDigit, true);
            if (m_cells2dArr[_posX + 1][_posY - 1]->getCellState() == eClosed)
                emptyCellsAnalizeVec.append(qMakePair(_posX + 1, _posY - 1));
            if (m_cells2dArr[_posX + 1][_posY - 1]->getCellState() == eDigit)
                m_cells2dArr[_posX + 1][_posY - 1]->setState(eDigit, true);
            if (m_cells2dArr[_posX + 1][_posY]->getCellState() == eClosed)
                emptyCellsAnalizeVec.append(qMakePair(_posX + 1, _posY));
            if (m_cells2dArr[_posX + 1][_posY]->getCellState() == eDigit)
                m_cells2dArr[_posX + 1][_posY]->setState(eDigit, true);
            if (m_cells2dArr[_posX + 1][_posY + 1]->getCellState() == eClosed)
                emptyCellsAnalizeVec.append(qMakePair(_posX + 1, _posY + 1));
            if (m_cells2dArr[_posX + 1][_posY + 1]->getCellState() == eDigit)
                m_cells2dArr[_posX + 1][_posY + 1]->setState(eDigit, true);
        }
        //проверка справа;
        else if (_posX == areaW)
        {
            if (m_cells2dArr[_posX][_posY - 1]->getCellState() == eClosed)
                emptyCellsAnalizeVec.append(qMakePair(_posX, _posY - 1));
            if (m_cells2dArr[_posX][_posY - 1]->getCellState() == eDigit)
                m_cells2dArr[_posX][_posY - 1]->setState(eDigit, true);
            if (m_cells2dArr[_posX][_posY + 1]->getCellState() == eClosed)
                emptyCellsAnalizeVec.append(qMakePair(_posX, _posY + 1));
            if (m_cells2dArr[_posX][_posY + 1]->getCellState() == eDigit)
                m_cells2dArr[_posX][_posY + 1]->setState(eDigit, true);
            if (m_cells2dArr[_posX - 1][_posY - 1]->getCellState() == eClosed)
                emptyCellsAnalizeVec.append(qMakePair(_posX - 1, _posY - 1));
            if (m_cells2dArr[_posX - 1][_posY - 1]->getCellState() == eDigit)
                m_cells2dArr[_posX - 1][_posY - 1]->setState(eDigit, true);
            if (m_cells2dArr[_posX - 1][_posY]->getCellState() == eClosed)
                emptyCellsAnalizeVec.append(qMakePair(_posX - 1, _posY));
            if (m_cells2dArr[_posX - 1][_posY]->getCellState() == eDigit)
                m_cells2dArr[_posX - 1][_posY]->setState(eDigit, true);
            if (m_cells2dArr[_posX - 1][_posY + 1]->getCellState() == eClosed)
                emptyCellsAnalizeVec.append(qMakePair(_posX - 1, _posY + 1));
            if (m_cells2dArr[_posX - 1][_posY + 1]->getCellState() == eDigit)
                m_cells2dArr[_posX - 1][_posY + 1]->setState(eDigit, true);
        }
        //проверка сверху;
        else if (_posY == 0)
        {
            if (m_cells2dArr[_posX - 1][_posY]->getCellState() == eClosed)
                emptyCellsAnalizeVec.append(qMakePair(_posX - 1, _posY));
            if (m_cells2dArr[_posX - 1][_posY]->getCellState() == eDigit)
                m_cells2dArr[_posX - 1][_posY]->setState(eDigit, true);
            if (m_cells2dArr[_posX + 1][_posY]->getCellState() == eClosed)
                emptyCellsAnalizeVec.append(qMakePair(_posX + 1, _posY));
            if (m_cells2dArr[_posX + 1][_posY]->getCellState() == eDigit)
                m_cells2dArr[_posX + 1][_posY]->setState(eDigit, true);
            if (m_cells2dArr[_posX - 1][_posY + 1]->getCellState() == eClosed)
                emptyCellsAnalizeVec.append(qMakePair(_posX - 1, _posY + 1));
            if (m_cells2dArr[_posX - 1][_posY + 1]->getCellState() == eDigit)
                m_cells2dArr[_posX - 1][_posY + 1]->setState(eDigit, true);
            if (m_cells2dArr[_posX][_posY + 1]->getCellState() == eClosed)
                emptyCellsAnalizeVec.append(qMakePair(_posX, _posY + 1));
            if (m_cells2dArr[_posX][_posY + 1]->getCellState() == eDigit)
                m_cells2dArr[_posX][_posY + 1]->setState(eDigit, true);
            if (m_cells2dArr[_posX + 1][_posY + 1]->getCellState() == eClosed)
                emptyCellsAnalizeVec.append(qMakePair(_posX + 1, _posY + 1));
            if (m_cells2dArr[_posX + 1][_posY + 1]->getCellState() == eDigit)
                m_cells2dArr[_posX + 1][_posY + 1]->setState(eDigit, true);
        }
        //проверка снизу;
        else if (_posY == areaH)
        {
            if (m_cells2dArr[_posX - 1][_posY]->getCellState() == eClosed)
                emptyCellsAnalizeVec.append(qMakePair(_posX - 1, _posY));
            if (m_cells2dArr[_posX - 1][_posY]->getCellState() == eDigit)
                m_cells2dArr[_posX - 1][_posY]->setState(eDigit, true);
            if (m_cells2dArr[_posX + 1][_posY]->getCellState() == eClosed)
                emptyCellsAnalizeVec.append(qMakePair(_posX + 1, _posY));
            if (m_cells2dArr[_posX + 1][_posY]->getCellState() == eDigit)
                m_cells2dArr[_posX + 1][_posY]->setState(eDigit, true);
            if (m_cells2dArr[_posX - 1][_posY - 1]->getCellState() == eClosed)
                emptyCellsAnalizeVec.append(qMakePair(_posX - 1, _posY - 1));
            if (m_cells2dArr[_posX - 1][_posY - 1]->getCellState() == eDigit)
                m_cells2dArr[_posX - 1][_posY - 1]->setState(eDigit, true);
            if (m_cells2dArr[_posX][_posY - 1]->getCellState() == eClosed)
                emptyCellsAnalizeVec.append(qMakePair(_posX, _posY - 1));
            if (m_cells2dArr[_posX][_posY - 1]->getCellState() == eDigit)
                m_cells2dArr[_posX][_posY - 1]->setState(eDigit, true);
            if (m_cells2dArr[_posX + 1][_posY - 1]->getCellState() == eClosed)
                emptyCellsAnalizeVec.append(qMakePair(_posX + 1, _posY - 1));
            if (m_cells2dArr[_posX + 1][_posY - 1]->getCellState() == eDigit)
                m_cells2dArr[_posX + 1][_posY - 1]->setState(eDigit, true);
        }

        //открываем клетку
        m_cells2dArr[_posX][_posY]->setState(eOpen, true);

        for(int i(0); i < emptyCellsAnalizeVec.size(); ++i)
        {
            foundEmptyCells(cordsToPos(emptyCellsAnalizeVec.at(i).first, emptyCellsAnalizeVec.at(i).second));
            emit imageUpdate();
        }
        emptyCellsAnalizeVec.clear();
    }

    //если бомба то открываем все бомбы;
    if (m_cells2dArr[_posX][_posY]->getCellState() == eBomb)
    {
        showAllBombs();
    }
    emit imageUpdate();
}


QPair<int, int> Minesweeper::posToCords(int position)
{
    QPair<int, int> coords;
    int counter = 0;
    for(int i(0); i < m_cellsWidthSize; ++i)
    {
        for(int j(0); j < m_cellsHeightSize; ++j)
        {
            if(position == counter)
            {
                coords.first = i;
                coords.second = j;
            }
            counter++;
        }
    }
    return coords;
}

int Minesweeper::cordsToPos(int posX, int posY)
{
    int result = 0;
    for(int i(0); i < m_cellsWidthSize; ++i)
    {
        for(int j(0); j < m_cellsHeightSize; ++j)
        {
            if(i == posX && j == posY)
            {
                i = m_cellsWidthSize;
                break;
            }
            result++;
        }
    }
    return result;
}

void Minesweeper::readSettings()
{
    m_settings.beginGroup("/MinesweeperQt/Settings");
    int widthArea = m_settings.value("/Width", m_cellsWidthSize).toInt();
    int heightArea = m_settings.value("/Height", m_cellsHeightSize).toInt();
    int bombsNumber = m_settings.value("/Bombs", m_bombsNumber).toInt();
    setAreaSize(widthArea, heightArea, bombsNumber);
    m_settings.endGroup();
}

void Minesweeper::writeSettings()
{
    m_settings.beginGroup("/MinesweeperQt/Settings");
    m_settings.setValue("Width", m_cellsWidthSize);
    m_settings.setValue("Height", m_cellsHeightSize);
    m_settings.setValue("Bombs", m_bombsNumber);
    m_settings.endGroup();
}

void Minesweeper::removeElement(QVector<QPair<int, int> > &pair, int pos1, int pos2)
{
    for(int i(0); i < pair.size(); ++i)
    {
        if(pair.at(i).first == pos1 && pair.at(i).second == pos2)
            pair.remove(i);
    }
}

void Minesweeper::showAllBombs()
{
    for(int i(0); i < m_cells2dArr.size(); ++i)
    {
        for(int j(0); j < m_cells2dArr.at(0).size(); ++j)
        {
            if (m_cells2dArr[i][j]->getCellState() == eBomb)
                m_cells2dArr[i][j]->setState(eBomb, true);
        }
        emit loseSignal();
    }
}

void Minesweeper::checkVictory()
{
    int openCells = 0;
    for (QVector<MCell*> &vec_current : m_cells2dArr)
    {
        for (MCell* value : vec_current)
        {
            if(value->getCellVisibility() == true)
                ++openCells;
        }
    }
    if(openCells >= (m_cellsWidthSize * m_cellsHeightSize) - m_bombsNumber)
        emit victorySignal();
}

Minesweeper::~Minesweeper()
{
    writeSettings();
}
