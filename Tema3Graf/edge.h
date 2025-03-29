#ifndef EDGE_H
#define EDGE_H

#include "node.h"
class Edge
{
public:
    Edge(Node f, Node s);
    Node getFirst() const;
    Node getSecond() const;

private:
    Node m_first;
    Node m_second;

};

#endif // EDGE_H
