#ifndef EDGE_H
#define EDGE_H

#include "node.h"
#include <QColor>

class Edge
{
public:
    Edge(Node f, Node s, int cost = 0);
    Node getFirst() const;
    Node getSecond() const;
    int getCost() const;
    void setCost(int cost);
    QColor getColor() const;
    void setColor(const QColor& color);
    bool operator==(const Edge& other) const
    {
        return (m_first == other.m_first && m_second == other.m_second) ||
               (m_first == other.m_second && m_second == other.m_first);
    }

private:
    Node m_first;
    Node m_second;
    int m_cost;
    QColor m_color;
};

#endif // EDGE_H
