#include "TopologicalGraphView.h"
#include <QtMath>
#include <QDebug>


TopologicalGraphView::TopologicalGraphView(const std::vector<Node>& nodes,
                                           const std::vector<Edge>& edges,
                                           QWidget *parent)
    : QWidget(parent), m_nodes(nodes), m_edges(edges)
{
    setFixedSize(800, 600);
}


void TopologicalGraphView::paintEvent(QPaintEvent *event) {
    QPainter p(this);


    QPen pen(Qt::white);
    p.setPen(pen);


    for (const auto& edge : m_edges) {
        int firstValue = edge.getFirst().getValue();
        int secondValue = edge.getSecond().getValue();


        if (firstValue < secondValue) {
            QPoint start = edge.getFirst().getPos();
            QPoint end = edge.getSecond().getPos();
            p.drawLine(start, end);
            drawArrow(&p, start, end);
            qDebug() << "Drawing edge: (" << firstValue << ", " << secondValue << ")";
        }
    }


    pen.setColor(Qt::white);
    p.setPen(pen);


    for (const auto& node : m_nodes) {
        QRect r(node.getPos().x() - 10, node.getPos().y() - 10, 20, 20);

        p.drawEllipse(r);

        QString str = QString::number(node.getValue());
        p.drawText(r, Qt::AlignCenter, str);
    }
}


void TopologicalGraphView::drawArrow(QPainter *painter, QPoint start, QPoint end) {
    double angle = std::atan2(end.y() - start.y(), end.x() - start.x());
    double arrowSize = 10.0;
    double arrowAngle = 30.0;

    QPoint arrowP1 = end - QPoint(std::cos(angle + qDegreesToRadians(arrowAngle)) * arrowSize,
                                  std::sin(angle + qDegreesToRadians(arrowAngle)) * arrowSize);
    QPoint arrowP2 = end - QPoint(std::cos(angle - qDegreesToRadians(arrowAngle)) * arrowSize,
                                  std::sin(angle - qDegreesToRadians(arrowAngle)) * arrowSize);

    painter->drawLine(end, arrowP1);
    painter->drawLine(end, arrowP2);
}

