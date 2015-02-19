#ifndef TAGBOARD_H
#define TAGBOARD_H

#include <vector>
#include <list>
#include <utility>
#include <ostream>
#include <istream>

class TagBoard
{
public:
    enum Move {left, right, top, bottom};

private:
    typedef std::pair<std::size_t, std::size_t> Position;
    typedef std::vector<int> Board;

    std::size_t _size;
    Board _board;
    Position _emptyCellPos;

    void resize(std::size_t size);
    int &Value(Position cellPos);
    int &Value(std::size_t first, std::size_t second);
    void swapCell(Position firstCell, Position secondCell);
public:    
    TagBoard() {}
    TagBoard(std::size_t size);
    TagBoard(const TagBoard& parent, Move move);

    bool makeMove(Move move);
    bool isCorrectMove(Move move) const;
    bool isSolutionExists() const;
    int getDistanceToVictory() const;
    int getDistanceToCell(int val, Position pos) const;
    int getDistanceToCell(int val, std::size_t first, std::size_t second) const;
    int getValue(std::size_t first, std::size_t second) const;

    bool operator == (const TagBoard& tag);
    bool operator != (const TagBoard& tag);

    friend std::ostream& operator << (std::ostream &out, const TagBoard& tag);
    friend std::istream &operator >> (std::istream &in, TagBoard& tag);
};

#endif // TAGBOARD_H
