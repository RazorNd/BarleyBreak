#include "tagboard.h"
#include <stdexcept>
#include <cmath>
#include <set>
#include <iomanip>

void TagBoard::resize(std::size_t size)
{
    _size = size;
    _board.resize(size * size);
}

int &TagBoard::Value(TagBoard::Position cellPos)
{
    if(cellPos.first >= _size || cellPos.second >= _size)
    {
        throw std::out_of_range("TagBoard::setCellValue out of range");
    }

    return _board[cellPos.first * _size + cellPos.second];
}

int &TagBoard::Value(std::size_t first, std::size_t second)
{
    return Value(Position(first, second));
}

void TagBoard::swapCell(TagBoard::Position firstCell, TagBoard::Position secondCell)
{
    std::swap(Value(firstCell), Value((secondCell)));
}

TagBoard::TagBoard(std::size_t size):
    _size(size)
{
    _board.resize(size * size);
}


TagBoard::TagBoard(const TagBoard &parent, TagBoard::Move move):
    _size(parent._size),
    _board(parent._board),
    _emptyCellPos(parent._emptyCellPos)
{
    makeMove(move);
}

bool TagBoard::makeMove(TagBoard::Move move)
{
    std::pair<std::size_t, std::size_t> newPos = _emptyCellPos;
    switch(move)
    {
    case top:
        if(_emptyCellPos.first > 0)
        {
            newPos.first--;
        }
        break;
    case bottom:
        if(_emptyCellPos.first < (_size - 1))
        {
            newPos.first++;
        }
        break;
    case left:
        if(_emptyCellPos.second > 0)
        {
            newPos.second--;
        }
        break;
    case right:
        if(_emptyCellPos.second < (_size - 1))
        {
            newPos.second++;
        }
    }
    if(newPos == _emptyCellPos)
    {
        return false;
    }

    swapCell(_emptyCellPos, newPos);
    _emptyCellPos = newPos;
    return true;
}

bool TagBoard::isCorrectMove(TagBoard::Move move) const
{
    switch(move)
    {
    case top:
        if(_emptyCellPos.first > 0)
        {
            return true;
        }
        break;
    case bottom:
        if(_emptyCellPos.first < (_size - 1))
        {
            return true;
        }
        break;
    case left:
        if(_emptyCellPos.second > 0)
        {
            return true;
        }
        break;
    case right:
        if(_emptyCellPos.second < (_size - 1))
        {
            return true;
        }
        break;
    }

    return false;
}

bool TagBoard::isSolutionExists() const
{
    int inv = 0;
    for(size_t i = 0; i < _board.size(); i++)
    {
        if(_board[i])
        {
            for (size_t j = 0; j < i; ++j)
                        if (_board[j] > _board[i])
                            ++inv;
        }
    }

    for (size_t i = 0; i < _board.size(); ++i)
        if (_board[i] == 0)
            inv += 1 + i / 4;

    return ((inv & 1) ? false : true);
}

int TagBoard::getDistanceToVictory() const
{
    int result = 0;
    for(std::size_t i = 0; i < _size; i++)
    {
        for(std::size_t j = 0; j < _size; j++)
        {
            result += getDistanceToCell(getValue(i, j), i, j);
        }
    }
    return result;
}

int TagBoard::getDistanceToCell(int val, TagBoard::Position pos) const
{
    return getDistanceToCell(val, pos.first, pos.second);
}

int TagBoard::getDistanceToCell(int val, std::size_t first, std::size_t second) const
{
    if(val <= 0 || val >= (int)(_size * _size))
        return 0;
    val--;

    const int correctFirst = val / _size;
    const int correctSecond = val % _size;

    const int divFirst = std::fabs(correctFirst - (int)first);
    const int divSecond = std::fabs(correctSecond - (int)second);

    return divFirst + divSecond;
}

int TagBoard::getValue(std::size_t first, std::size_t second) const
{
    if(first >= _size || second >= _size)
    {
        throw std::length_error("TagBoard::setCellValue out of range");
    }

    return _board.at(first * _size + second);
}

bool TagBoard::operator ==(const TagBoard &tag)
{
    if(_size != tag._size)
        return false;

    for(Board::const_iterator it1 = _board.begin(), it2 = tag._board.begin();
        it1 != _board.end(); it1++, it2++)
    {
        if((*it1) != (*it2))
            return false;
    }

    return true;
}

bool TagBoard::operator !=(const TagBoard &tag)
{
    return !((*this) == tag);
}

std::istream &operator >>(std::istream &in, TagBoard &tag)
{
    std::size_t size;
    in >> size;
    const int maxValue = size * size;
    std::set<int> checkDuplicate;
    if(size != tag._size)
    {
        tag.resize(size);
    }
    TagBoard::Board::iterator it = tag._board.begin();
    for(std::size_t i = 0; i < size; i++)
    {
        for(std::size_t j = 0; j < size; j++, it++)
        {
            int val;
            in >> val;

            if(val >= maxValue)
                throw std::invalid_argument("TagBoard set wrong value: value more then Max Value");

            if(checkDuplicate.find(val) == checkDuplicate.end())
                checkDuplicate.insert(val);
            else
                throw std::invalid_argument("TagBoard set wrong value: replay value");

            if(val == 0)
            {
                tag._emptyCellPos = TagBoard::Position(i, j);
            }

            (*it) = val;
        }
    }
    return in;
}

std::ostream &operator<<(std::ostream &out, const TagBoard& tag)
{
    const std::size_t size = tag._size;

    //out << "Size is: " << size << std::endl;

    for(std::size_t i = 0; i < size; i++)
    {
        out << "| ";
        for(std::size_t j = 0; j < size; j++)
        {
            int val = tag.getValue(i, j);
            if(val)
                out << std::setw(4) << val << " | ";
            else
                out << std::setw(4) << " " << " | ";
        }
        out << std::endl;
    }

    out << "Distance to victory is: " << tag.getDistanceToVictory() << std::endl;
    return out;
}
