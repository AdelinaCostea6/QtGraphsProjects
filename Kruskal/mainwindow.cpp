#include "mainwindow.h"
#include "./ui_mainwindow.h"
#include <QRadioButton>
#include <QButtonGroup>
#include <QPainterPath>
#include "costdialog.h"
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

    radioUnoriented->setChecked(true);
    QPushButton *button = new QPushButton("Calculare MST", this);
    button->setGeometry(QRect(QPoint(200, 100), QSize(200, 50)));

    connect(button, &QPushButton::clicked, this, &MainWindow::onMinimumSpanningTreeClicked);
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

                    CostDialog costDialog;
                    if (costDialog.exec() == QDialog::Accepted) {
                        int cost = costDialog.getCost();
                        if (cost != -1) {

                            m_graph.addEdge(m_selectedNode, n, cost);

                            qDebug() << "Costul introdus este: " << cost;
                        }
                    }
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


    QColor mstColor = Qt::red;

    for (const auto& e : edges)
    {
        QPoint start = e.getFirst().getPos();
        QPoint end = e.getSecond().getPos();


        bool isMSTEdge = std::find(m_mstEdges.begin(), m_mstEdges.end(), e) != m_mstEdges.end();

        if (isMSTEdge)
        {
            p.setPen(QPen(mstColor));
        }
        else
        {
            p.setPen(QPen(Qt::white));
        }


        p.drawLine(start, end);

        QPoint mid((start.x() + end.x()) / 2, (start.y() + end.y()) / 2);
        QString costText = QString::number(e.getCost());
        p.drawText(mid, costText);

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

void MainWindow::onMinimumSpanningTreeClicked()
{
    if (m_graph.isOriented) {
        QMessageBox::warning(this, "Eroare", "Calculul arborelui cu cost minim este permis doar pentru grafuri neorientate.");
        return;
    }

    if(!m_graph.isConnected()) {
        QMessageBox::warning(this, "Eroare", "Graful trebuie să fie conex pentru a calcula arborele cu cost minim.");
        return;
    }

    m_mstEdges = m_graph.getMstEdgesGeneric();
    m_graph.setMstEdgesGeneric(m_mstEdges);

    update();
}


