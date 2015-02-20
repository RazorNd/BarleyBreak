#include "findertaggame.h"


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

std::ostream &operator <<(std::ostream &out, const FinderTagGame::NodeTag &node)
{
    out << "The objective function: " << node() << std::endl
        << node.getTagBoard() << node.getMove() << std::endl;
    return out;
}

void FinderTagGame::addNodeInPull(FinderTagGame::NodeTag *node)
{
    LinkNodeTag link(node);
    _pool.push_back(node);
    _nodes.insert(link);
}

FinderTagGame::NodeTag &FinderTagGame::createNode(const FinderTagGame::NodeTag *parent, TagBoard::Move move)
{
    NodeTag *node = new NodeTag(parent, move);
    addNodeInPull(node);
    return *node;
}

void FinderTagGame::createNode(const TagBoard &tag)
{
    NodeTag* node = new NodeTag(tag);
    addNodeInPull(node);
}

const FinderTagGame::NodeTag *FinderTagGame::makeDecisionTree()
{
    const NodeTag *nodeAnswer = 0;

    while(!_nodes.empty() && !nodeAnswer)
    {
        Nodes::iterator it = _nodes.begin();
        NodeTag *node = it->node();

        _nodes.erase(it);
        for(int i = 0; i < 4; i++)
        {
            TagBoard::Move move = (TagBoard::Move)i;
            if(node->isCorrectMove(move) && !TagBoard::isTurnBack(move, node->getMove()))
            {
                NodeTag &newNode = createNode(node, (TagBoard::Move)i);
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

FinderTagGame::~FinderTagGame()
{
    for(NodesPool::iterator it = _pool.begin(); it != _pool.end(); it++)
    {
        delete (*it);
    }
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

FinderTagGame::LinkNodeTag::LinkNodeTag(NodeTag *obj):
    _link(obj)
{
}

FinderTagGame::NodeTag *FinderTagGame::LinkNodeTag::node() const
{
    return _link;
}

bool FinderTagGame::LinkNodeTag::operator <(const LinkNodeTag &a) const
{
    return (*_link) < (*a._link);
}

bool FinderTagGame::LinkNodeTag::operator <=(const FinderTagGame::LinkNodeTag &a) const
{
    return (*_link) <= (*a._link);
}
