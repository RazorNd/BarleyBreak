#ifndef FINDERTAGGAME_H
#define FINDERTAGGAME_H

#include <tagboard.h>
#include <set>
#include <list>
#include <istream>

class FinderTagGame
{
    class NodeTag;
    class LinkNodeTag;
    typedef std::set<LinkNodeTag, std::less_equal<LinkNodeTag> > Nodes;
    typedef std::list<NodeTag*> NodesPool;
    Nodes _nodes;
    NodesPool _pool;

    void addNodeInPull(NodeTag* node);
    NodeTag& createNode(const NodeTag* parent, TagBoard::Move move);
    void createNode(const TagBoard &tag);
public:
    FinderTagGame(std::istream &input);
    ~FinderTagGame();
    std::list<TagBoard::Move> getMoveList();
};

class FinderTagGame::LinkNodeTag
{
    FinderTagGame::NodeTag* _link;
public:
    LinkNodeTag(NodeTag *obj);
    FinderTagGame::NodeTag *node() const;

    bool operator < (const LinkNodeTag &a) const;
    bool operator <= (const LinkNodeTag &a) const;
};

class FinderTagGame::NodeTag
{
    TagBoard _board;
    const NodeTag* _parent;
    int _length;
    TagBoard::Move _move;
public:
    NodeTag(const TagBoard &tag);
    NodeTag(const NodeTag* parent, TagBoard::Move move);

    const NodeTag *getParent() const;
    int getDistanceToVicktory() const;
    TagBoard::Move getMove() const;
    bool isCorrectMove(TagBoard::Move move) const;
    const TagBoard &getTagBoard() const;

    int operator() () const;
    bool operator < (const NodeTag& tag) const;
    bool operator <= (const NodeTag& tag) const;
};

#endif // FINDERTAGGAME_H
