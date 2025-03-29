#include "mainwindow.h"
#include "./ui_mainwindow.h"
#include <QRadioButton>
#include <QButtonGroup>
#include <QPainterPath>

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

    QPushButton *checkTreeButton = new QPushButton("Check if Tree", this);
    checkTreeButton->setGeometry(QRect(50, 150, 120, 30));
    connect(checkTreeButton, &QPushButton::clicked, this, &MainWindow::checkIfTree);
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

void MainWindow::checkIfTree()
{
    if (m_graph.isTree())
    {
        qDebug()<<m_graph.findRoot();
        QMessageBox::information(this, "Check Tree", "Graful este un arbore.");
    } else {
        QMessageBox::information(this, "Check Tree", "Graful nu este un arbore.");
    }
}

