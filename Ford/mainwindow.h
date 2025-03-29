#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QMainWindow>
#include <QMouseEvent>
#include <QPainter>
#include "graph.h"

QT_BEGIN_NAMESPACE
namespace Ui {
class MainWindow;
}
QT_END_NAMESPACE

class MainWindow : public QMainWindow
{
    Q_OBJECT

public:
    MainWindow(QWidget *parent = nullptr);
    ~MainWindow();
    void mouseReleaseEvent(QMouseEvent *event) override;
    void paintEvent(QPaintEvent *event) override;
    void drawArrow(QPainter *painter, QPoint start, QPoint end);
    void changeGraph(bool isOriented);
    void mousePressEvent(QMouseEvent *event) override;
    void mouseMoveEvent(QMouseEvent *event) override;
    void onMaxFlowButtonClicked();
    void onRedrawGraphButtonClicked();
private:
    Ui::MainWindow *ui;
    Graph m_graph;
    Node m_selectedNode;
    bool m_isFirstNodeSelected;
    int m_selectedNodeIndex;
    int draggedNodeIndex=-1;
    bool m_isMovingNode;
    bool m_isSelectingSourceOrSink = false;
    int selectedSource = -1;
    int selectedSink = -1;
  bool isUpdating = false;

};
#endif // MAINWINDOW_H
