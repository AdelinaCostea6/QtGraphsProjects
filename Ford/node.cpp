#include "node.h"

Node::Node()
{
}

int Node::getValue() const
{
    return m_value;
}


void Node::setValue(int n)
{
    m_value=n;
}

void Node::setPos(const QPoint& p)
{
    m_pos=p;
}

bool Node::operator==(const Node &other) const
{
    return m_value==other.m_value;
}

QPoint Node::getPos() const
{
    return m_pos;
}
