#include "edge.h"



Edge::Edge(Node f, Node s, int cost)
    : m_first(f), m_second(s), m_cost(cost), m_color(Qt::white) // Culoare implicită este neagră
{
}

Node Edge::getFirst() const
{
    return m_first;
}

Node Edge::getSecond() const
{
    return m_second;
}

int Edge::getCost() const
{
    return m_cost;
}

void Edge::setCost(int cost)
{
    m_cost = cost;
}

QColor Edge::getColor() const
{
    return m_color;
}

void Edge::setColor(const QColor& color)
{
    m_color = color;
}
