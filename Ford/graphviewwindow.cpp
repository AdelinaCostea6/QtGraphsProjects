#include "GraphViewWindow.h"
#include <QGraphicsEllipseItem>
#include <QGraphicsTextItem>
#include <QGraphicsLineItem>
#include <QPen>
#include <QBrush>

GraphViewWindow::GraphViewWindow(Graph& graph, QWidget* parent)
    : QWidget(parent), m_graph(graph)
{
    m_scene = new QGraphicsScene(this);
    m_view = new QGraphicsView(m_scene, this);

    setWindowTitle("Updated Graph View");
    resize(800, 600);
    m_view->setRenderHint(QPainter::Antialiasing);
    m_view->setSceneRect(0, 0, 800, 600);

    drawGraph();
}

void GraphViewWindow::drawGraph()
{
    const std::vector<Node>& nodes = m_graph.getNodes();
    const std::vector<std::vector<int>>& matrix = m_graph.getresidualGraph();

    const int radius = 20;
    QBrush nodeBrush(Qt::blue);
    QPen edgePen(Qt::black);
    edgePen.setWidth(2);

    // Desenează nodurile
    for (const auto& node : nodes)
    {
        QPointF pos(node.getPos().x(), node.getPos().y());
        QGraphicsEllipseItem* circle = m_scene->addEllipse(pos.x() - radius, pos.y() - radius, 2 * radius, 2 * radius, QPen(), nodeBrush);
        QGraphicsTextItem* label = m_scene->addText(QString::number(node.getValue()));
        label->setPos(pos.x() - radius / 2, pos.y() - radius / 2);
    }

    // Desenează muchiile
    for (int i = 0; i < nodes.size(); ++i)
    {
        for (int j = 0; j < nodes.size(); ++j)
        {
            if (matrix[i][j] > 0)  // Dacă există o muchie
            {
                QPointF pos1(nodes[i].getPos().x(), nodes[i].getPos().y());
                QPointF pos2(nodes[j].getPos().x(), nodes[j].getPos().y());

                QGraphicsLineItem* line = m_scene->addLine(pos1.x(), pos1.y(), pos2.x(), pos2.y(), edgePen);

                // Afișează capacitatea
                QPointF midPoint((pos1.x() + pos2.x()) / 2, (pos1.y() + pos2.y()) / 2);
                QGraphicsTextItem* capacity = m_scene->addText(QString::number(matrix[i][j]));
                capacity->setDefaultTextColor(Qt::red);
                capacity->setPos(midPoint.x(), midPoint.y());
            }
        }
    }
}
