#include "findertaggame.h"
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
    //return _board.getDistanceToVictory();
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

FinderTagGame::FinderTagGame(std::istream &input)
{
    TagBoard initialTag;
    input >> initialTag;

    if(initialTag.isSolutionExists())
        createNode(initialTag);
    else
    {
        std::cout << "No Solution";
    }
}

FinderTagGame::~FinderTagGame()
{
    for(NodesPool::iterator it = _pool.begin(); it != _pool.end(); it++)
    {
        delete (*it);
    }
}

std::list<TagBoard::Move> FinderTagGame::getMoveList()
{
    bool endsSearch = false;
    const NodeTag *nodeAnswer;

    int minDistanceToVicktory = 1000000;
    while(!_nodes.empty() && !endsSearch)
    {
        Nodes::iterator it = _nodes.begin();
        NodeTag *node = it->node();

        _nodes.erase(it);

        if(minDistanceToVicktory > node->getDistanceToVicktory() || minDistanceToVicktory <= 3)
        {
            minDistanceToVicktory = node->getDistanceToVicktory();
            std::cout << "The objective function: " << (*node)()
                      << " Nodes count: " << _nodes.size() << std::endl
                      << node->getTagBoard() << std::endl;
        }
//        std::cout << "Min distance to victory " << minDistanceToVicktory
//                  << " Distance to vickory: " << node->getDistanceToVicktory()
//                  << " Nodes count: " << _nodes.size() << std::endl;

        for(int i = 0; i < 4; i++)
        {
            if(node->isCorrectMove((TagBoard::Move)i) && (TagBoard::Move)i != node->getMove())
            {
                NodeTag &newNode = createNode(node, (TagBoard::Move)i);
                if(!newNode.getDistanceToVicktory())
                {
                    endsSearch = true;
                    nodeAnswer = &newNode;
                    break;
                }
            }
        }
    }

    std::list<TagBoard::Move> result;
    while(nodeAnswer->getParent())
    {
        result.push_front(nodeAnswer->getMove());
        nodeAnswer = nodeAnswer->getParent();
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
