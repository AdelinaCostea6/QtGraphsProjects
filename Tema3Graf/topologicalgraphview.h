#ifndef TOPOLOGICALGRAPHVIEW_H
#define TOPOLOGICALGRAPHVIEW_H

#include <QWidget>
#include <QPainter>
#include <QPoint>
#include <vector>
#include "Node.h"
#include "Edge.h"

class TopologicalGraphView : public QWidget
{
    Q_OBJECT

public:
    TopologicalGraphView(const std::vector<Node>& nodes,
                         const std::vector<Edge>& edges,
                         QWidget *parent = nullptr);

protected:
    void paintEvent(QPaintEvent *event) override;

private:
    std::vector<Node> m_nodes;
    std::vector<Edge> m_edges;

    void drawArrow(QPainter *painter, QPoint start, QPoint end);

};

#endif // TOPOLOGICALGRAPHVIEW_H
