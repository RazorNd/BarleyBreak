#ifndef FINDERTAGGAME_H
#define FINDERTAGGAME_H

#include <tagboard.h>
#include <set>
#include <unordered_set>
#include <list>
#include <istream>
#include <ostream>

class FinderTagGame
{
public:
    typedef std::list<TagBoard::Move> TagMoveList;
    typedef std::list<TagBoard> TagBoardList;
private:
    class NodeTag
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
        bool operator == (const NodeTag& tag) const;
    };
    class NodeTagPool
    {
        class NodeTagHash
        {
        public:
            std::size_t operator() (const NodeTag &tag) const;
        };
        typedef std::unordered_set<FinderTagGame::NodeTag, NodeTagHash> Pool;
        mutable Pool _nodesTag;
    public:
        const FinderTagGame::NodeTag &get(const NodeTag *parent, TagBoard::Move move) const;
        const FinderTagGame::NodeTag &get(const TagBoard &board);
        const FinderTagGame::NodeTag &get(NodeTag &&tag) const;
        std::size_t poolSize() const;
    };
    class NodeTagPtr
    {
        const FinderTagGame::NodeTag& _link;
    public:
        NodeTagPtr(const NodeTag &obj);
        const FinderTagGame::NodeTag &node() const;

        bool operator < (const NodeTagPtr &a) const;
        bool operator <= (const NodeTagPtr &a) const;
    };

    typedef std::multiset<NodeTagPtr, std::less_equal<NodeTagPtr> > Nodes;
    Nodes _nodes;
    NodeTagPool _pool;
    const NodeTag *_nodeAnswer;

    void addPointerInQueue(const NodeTag &node);
    const NodeTag& createNode(const NodeTag &parent, TagBoard::Move move);
    void createNode(const TagBoard &tag);
    const NodeTag *makeDecisionTree();
public:
    FinderTagGame(std::istream &input);
    FinderTagGame(const TagBoard &initialTag);    
    TagMoveList getMoveList();    

    friend std::ostream &operator << (std::ostream &out, const NodeTag& node);
};

std::ostream &operator << (std::ostream &out, const FinderTagGame::NodeTag& node);


#endif // FINDERTAGGAME_H
