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
    enum Move {left, right, top, bottom, notCorrect};

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

    bool operator == (const TagBoard& tag) const;
    bool operator != (const TagBoard& tag) const;

    static bool isTurnBack(Move a, Move b);

    friend std::ostream& operator << (std::ostream &out, const TagBoard& tag);
    friend std::istream &operator >> (std::istream &in, TagBoard& tag);
    friend struct std::hash<TagBoard>;
};

std::ostream &operator<<(std::ostream &out, const TagBoard::Move& move);

namespace std {
template<>
struct hash<TagBoard>
{
    std::size_t operator() (const TagBoard &tag) const;
};
}

#endif // TAGBOARD_H
