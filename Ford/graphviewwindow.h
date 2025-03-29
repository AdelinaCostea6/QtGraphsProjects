#ifndef GRAPHVIEWWINDOW_H
#define GRAPHVIEWWINDOW_H

#include <QWidget>
#include <QGraphicsScene>
#include <QGraphicsView>
#include "graph.h"

class GraphViewWindow : public QWidget
{
    Q_OBJECT

public:
    explicit GraphViewWindow(Graph& graph, QWidget* parent = nullptr);

private:
    QGraphicsScene* m_scene;
    QGraphicsView* m_view;
    Graph& m_graph;

    void drawGraph();
};

#endif // GRAPHVIEWWINDOW_H
