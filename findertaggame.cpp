#include "findertaggame.h"
#include <unordered_set>
#include <iostream>

FinderTagGame::NodeTag::NodeTag(const TagBoard &tag):
    _parent(0), _length(0)
{
    _board = tag;
}

FinderTagGame::NodeTag::NodeTag(const FinderTagGame::NodeTag *parent, TagBoard::Move move):
    _parent(parent), _move(move)
{
    if(parent)
    {
        _board = TagBoard(parent->_board, move);
        _length = parent->_length + 1;
    }
}

const FinderTagGame::NodeTag *FinderTagGame::NodeTag::getParent() const
{
    return _parent;
}

int FinderTagGame::NodeTag::getDistanceToVicktory() const
{
    return _board.getDistanceToVictory();
}

TagBoard::Move FinderTagGame::NodeTag::getMove() const
{
    return _move;
}

bool FinderTagGame::NodeTag::isCorrectMove(TagBoard::Move move) const
{
    return _board.isCorrectMove(move);
}

const TagBoard &FinderTagGame::NodeTag::getTagBoard() const
{
    return _board;
}

int FinderTagGame::NodeTag::operator()() const
{    
    return _length + _board.getDistanceToVictory();    
}

bool FinderTagGame::NodeTag::operator <(const FinderTagGame::NodeTag &tag) const
{
    if((*this)() < tag())
        return true;
    if((*this)() == tag())
        return this->getDistanceToVicktory()< tag.getDistanceToVicktory();
    return false;
}

bool FinderTagGame::NodeTag::operator <=(const FinderTagGame::NodeTag &tag) const
{
    if((*this)() < tag())
        return true;
    if((*this)() == tag() && this->getDistanceToVicktory() < tag.getDistanceToVicktory())
        return true;
    if(this->getDistanceToVicktory() == tag.getDistanceToVicktory())
        return this->_length <= tag._length;
    return false;
}

bool FinderTagGame::NodeTag::operator ==(const FinderTagGame::NodeTag &tag) const
{
    return getTagBoard() == tag.getTagBoard();
}

std::ostream &operator <<(std::ostream &out, const FinderTagGame::NodeTag &node)
{
    out << "The objective function: " << node() << std::endl
        << node.getTagBoard() << node.getMove() << std::endl;
    return out;
}

void FinderTagGame::addPointerInQueue(const FinderTagGame::NodeTag &node)
{
    NodeTagPtr link(node);
    _nodes.insert(link);
}

const FinderTagGame::NodeTag &FinderTagGame::createNode(const FinderTagGame::NodeTag &parent, TagBoard::Move move)
{
    const NodeTag &node = _pool.get(&parent, move);
    addPointerInQueue(node);
    return node;
}

void FinderTagGame::createNode(const TagBoard &tag)
{
    const NodeTag &node = _pool.get(tag);
    addPointerInQueue(node);
}

const FinderTagGame::NodeTag *FinderTagGame::makeDecisionTree()
{
    const NodeTag *nodeAnswer = 0;

    while(!_nodes.empty() && !nodeAnswer)
    {
        Nodes::iterator it = _nodes.begin();
        const NodeTag &node = it->node();
        std::cout << _pool.poolSize() << std::endl;

        _nodes.erase(it);
        for(int i = 0; i < 4; i++)
        {
            TagBoard::Move move = (TagBoard::Move)i;
            if(node.isCorrectMove(move) && !TagBoard::isTurnBack(move, node.getMove()))
            {
                const NodeTag &newNode = createNode(node, (TagBoard::Move)i);
                if(!newNode.getDistanceToVicktory())
                {
                    nodeAnswer = &newNode;
                    break;
                }
            }
        }
    }
    return nodeAnswer;
}

FinderTagGame::FinderTagGame(std::istream &input)
{
    TagBoard initialTag;
    input >> initialTag;

    if(initialTag.isSolutionExists())
        createNode(initialTag);

    _nodeAnswer = makeDecisionTree();
}

FinderTagGame::FinderTagGame(const TagBoard &initialTag)
{
    if(initialTag.isSolutionExists())
        createNode(initialTag);

    _nodeAnswer = makeDecisionTree();
}

FinderTagGame::TagMoveList FinderTagGame::getMoveList()
{
    if(!_nodeAnswer)
        return TagMoveList();

    const NodeTag *node = _nodeAnswer;

    TagMoveList result;
    while(node->getParent())
    {
        result.push_front(node->getMove());
        node = node->getParent();
    }
    return result;
}

FinderTagGame::NodeTagPtr::NodeTagPtr(const NodeTag &obj):
    _link(obj)
{
}

const FinderTagGame::NodeTag &FinderTagGame::NodeTagPtr::node() const
{
    return _link;
}

bool FinderTagGame::NodeTagPtr::operator <(const NodeTagPtr &a) const
{
    return _link < a._link;
}

bool FinderTagGame::NodeTagPtr::operator <=(const FinderTagGame::NodeTagPtr &a) const
{
    return _link <= a._link;
}

const FinderTagGame::NodeTag &FinderTagGame::NodeTagPool::get(const NodeTag *parent, TagBoard::Move move) const
{
    return get(NodeTag(parent, move));
}

const FinderTagGame::NodeTag &FinderTagGame::NodeTagPool::get(const TagBoard &board)
{
    return get(NodeTag(board));
}

const FinderTagGame::NodeTag &FinderTagGame::NodeTagPool::get(NodeTag &&tag) const
{
    auto result = _nodesTag.emplace(tag);
    return *result.first;
}

std::size_t FinderTagGame::NodeTagPool::poolSize() const
{
    return _nodesTag.size();
}

std::size_t FinderTagGame::NodeTagPool::NodeTagHash::operator()(const NodeTag &tag) const
{
    std::hash<TagBoard> hash;
    return hash(tag.getTagBoard());
}
