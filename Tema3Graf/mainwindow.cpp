#include "mainwindow.h"
#include "./ui_mainwindow.h"
#include <QRadioButton>
#include <QButtonGroup>
#include <QPainterPath>
#include <QPushButton>
#include <QMessageBox>

MainWindow::MainWindow(QWidget *parent)
    : QMainWindow(parent), m_isMovingNode(false)
    , ui(new Ui::MainWindow)
{
    ui->setupUi(this);

    m_isFirstNodeSelected = false;

    QRadioButton *radioOriented = new QRadioButton("Oriented", this);
    QRadioButton *radioUnoriented = new QRadioButton("Unoriented", this);

    radioOriented->setGeometry(QRect(50, 50, 100, 30));
    radioUnoriented->setGeometry(QRect(50, 100, 100, 30));

    QButtonGroup *buttonGroup = new QButtonGroup(this);
    buttonGroup->addButton(radioOriented);
    buttonGroup->addButton(radioUnoriented);

    QPushButton *dfsButton = new QPushButton("Parcurgere în adâncime", this);
    dfsButton->setGeometry(QRect(50, 150, 200, 30));

    connect(dfsButton, &QPushButton::clicked, this, &MainWindow::onDepthFirstSearchButtonClicked);

    QPushButton *cycleButton = new QPushButton("Check for Cycles", this);
    cycleButton->setGeometry(QRect(50, 200, 150, 30));

    connect(cycleButton, &QPushButton::clicked, this, &MainWindow::onCycleButtonClicked);

    QPushButton *topologicalButton = new QPushButton("Topological Sort", this);
    topologicalButton->setGeometry(QRect(50, 250, 150, 30));

    connect(topologicalButton, &QPushButton::clicked, this, &MainWindow::onTopologicalSortButtonClicked);

    radioUnoriented->setChecked(true);
    qDebug()<<"Grph is now unoriented\n";

    connect(radioOriented, &QRadioButton::toggled, this, [this](bool checked)
            {
                if (checked)
                {
                    m_graph.setOriented(true);
                    qDebug() << "Graph is now oriented";
                    changeGraph(true);
                }
            });

    connect(radioUnoriented, &QRadioButton::toggled, this, [this](bool checked)
            {
                if (checked)
                {
                    m_graph.setOriented(false);
                    qDebug() << "Graph is now unoriented";
                    changeGraph(false);
                }
            });
    m_graph.depthFirstSearch(1, "dfs_output.txt"); // Începe parcurgerea de la nodul 1
}

MainWindow::~MainWindow()
{
    delete ui;
}

void MainWindow::mouseReleaseEvent(QMouseEvent *event)
{
    if (event->button() == Qt::RightButton)
    {
        m_graph.addNode(event->pos());
        update();
    }
    else if (event->button() == Qt::LeftButton)
    {
        std::vector<Node>& nodes = m_graph.getNodes();
        for (auto& n : nodes)
        {
            if (((n.getPos().x() - event->pos().x()) * (n.getPos().x() - event->pos().x()) +
                 (n.getPos().y() - event->pos().y()) * (n.getPos().y() - event->pos().y())) <= 100)
            {
                if (!m_isFirstNodeSelected)
                {
                    m_selectedNode = n;
                    m_isFirstNodeSelected = true;
                }
                else
                {
                    m_graph.addEdge(m_selectedNode, n);
                    m_isFirstNodeSelected = false;
                    update();
                }
                break;
            }
        }
    }


    draggedNodeIndex = -1;
    update();
}


void MainWindow::paintEvent(QPaintEvent *event)
{
    QPainter p(this);
    std::vector<Node>& nodes = m_graph.getNodes();
    std::vector<Edge>& edges = m_graph.getEdges();

    if (edges.empty() && nodes.empty()) return;

    for (const auto& e : edges)
    {
        QPoint start = e.getFirst().getPos();
        QPoint end = e.getSecond().getPos();
        p.drawLine(start, end);

        if (m_graph.isOrient())
        {
            drawArrow(&p, start, end);
        }
    }


    for (const auto& n : nodes)
    {
        QRect r(n.getPos().x() - 10, n.getPos().y() - 10, 20, 20);
        p.drawEllipse(r);
        QString str = QString::number(n.getValue());
        p.drawText(r, Qt::AlignCenter, str);
    }
}

void MainWindow::drawArrow(QPainter *painter, QPoint start, QPoint end)
{

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

void MainWindow::changeGraph(bool isOriented)
{

    m_graph.setOriented(isOriented);
    m_graph.clearAll();
    update();
}

void MainWindow::mousePressEvent(QMouseEvent *event)
{
    if (event->button() == Qt::LeftButton)
    {
        for (int i = 0; i < m_graph.getNodes().size(); ++i)
        {
            Node node = m_graph.getNodes()[i];
            QRect nodeRect(node.getPos().x() - 10, node.getPos().y() - 10, 20, 20);
            if (nodeRect.contains(event->pos()))
            {
                draggedNodeIndex = static_cast<int>(i);
                m_isMovingNode = true;
                break;
            }
        }
    }
}

void MainWindow::mouseMoveEvent(QMouseEvent *event)
{
    if (draggedNodeIndex != -1)
    {
        QPoint newPos = event->pos();
        m_graph.getNodes()[draggedNodeIndex].setPos(newPos);
        update();
    }
}

void MainWindow::onDepthFirstSearchButtonClicked()
{

    if (m_graph.getNodes().empty()) {
        qDebug() << "Graf gol! Adaugă noduri înainte de a începe parcurgerea.";
        return;
    }

    int startNodeValue = m_graph.getNodes().front().getValue();


    m_graph.depthFirstSearch(startNodeValue, "dfs_output.txt");

    qDebug() << "Parcurgerea în adâncime a fost salvată în dfs_output.txt";
}

void MainWindow::onCycleButtonClicked() {
    if (m_graph.hasCycle()) {
        QMessageBox::information(this, "Cycle Detection", "The graph contains a cycle.");
    } else {
        QMessageBox::information(this, "Cycle Detection", "The graph does not contain any cycles.");
    }
}

void MainWindow::onTopologicalSortButtonClicked() {
    if (m_graph.topologicalSortToFile("topological_sort_result.txt")) {

        TopologicalGraphView* graphView = new TopologicalGraphView(m_graph.getNodes(), m_graph.getEdges());
        graphView->show();
    }
}
