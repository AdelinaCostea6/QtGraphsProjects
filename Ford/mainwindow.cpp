#include "mainwindow.h"
#include "./ui_mainwindow.h"
#include <QRadioButton>
#include <QButtonGroup>
#include <QPainterPath>
#include "costdialog.h"
#include <QMessageBox>
#include <QLabel>
#include "graphviewwindow.h"
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

    QPushButton* maxFlowButton = new QPushButton("Calculați Fluxul Maxim", this);
    maxFlowButton->setGeometry(50, 150, 200, 40);
    connect(maxFlowButton, &QPushButton::clicked, this, &MainWindow::onMaxFlowButtonClicked);

    QPushButton* redrawButton = new QPushButton("Redesenare Grafic", this);
    redrawButton->setGeometry(50, 250, 200, 40);
    connect(redrawButton, &QPushButton::clicked, this, &MainWindow::onRedrawGraphButtonClicked);

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


        if (m_isSelectingSourceOrSink) {

            for (auto& n : nodes)
            {
                if (((n.getPos().x() - event->pos().x()) * (n.getPos().x() - event->pos().x()) +
                     (n.getPos().y() - event->pos().y()) * (n.getPos().y() - event->pos().y())) <= 100)
                {

                    if (selectedSource == -1) {
                        selectedSource = n.getValue();
                        qDebug() << "Nodul sursă ales: " << selectedSource;
                    }

                    else if (selectedSink == -1 && n.getValue() != selectedSource) {
                        selectedSink = n.getValue();
                        qDebug() << "Nodul destinație ales: " << selectedSink;
                    }


                    if (selectedSource != -1 && selectedSink != -1) {
                        int maxFlow = m_graph.fordFulkerson(selectedSource - 1, selectedSink - 1);
                        qDebug() << "Fluxul maxim este: " << maxFlow;


                        QLabel* resultLabel = new QLabel(this);
                        resultLabel->setText("Fluxul Maxim: " + QString::number(maxFlow));
                        resultLabel->setGeometry(50, 200, 200, 40);
                        resultLabel->show();


                        m_isSelectingSourceOrSink = false;
                        selectedSource = -1;
                        selectedSink = -1;
                    }
                    break;
                }
            }
        }

        else {
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


    const std::vector<Edge>& minCutEdges = m_graph.getMinCutEdges();

    for (const auto& e : edges)
    {
        QPoint start = e.getFirst().getPos();
        QPoint end = e.getSecond().getPos();

        bool isMinCut = false;
        for (const auto& minCutEdge : minCutEdges) {
            if ((minCutEdge.getFirst() == e.getFirst() && minCutEdge.getSecond() == e.getSecond()) ||
                (minCutEdge.getFirst() == e.getSecond() && minCutEdge.getSecond() == e.getFirst())) {
                isMinCut = true;
                break;
            }
        }

        if (isMinCut) {
            p.setPen(QPen(Qt::red));
        } else {
            p.setPen(QPen(Qt::white));
        }

        p.drawLine(start, end);

        QPoint mid((start.x() + end.x()) / 2, (start.y() + end.y()) / 2);
        QString costText = QString::number(e.getCost());
        p.drawText(mid, costText);

        if (m_graph.isOrient()) {
            drawArrow(&p, start, end);
        }
    }

    for (const auto& n : nodes)
    {
        QRect r(n.getPos().x() - 10, n.getPos().y() - 10, 20, 20);


        if (n.getValue() == selectedSource) {
            p.setPen(QPen(Qt::green));
        } else if (n.getValue() == selectedSink) {
            p.setPen(QPen(Qt::green));
        } else {
            p.setPen(QPen(Qt::white));
        }

        p.setBrush(Qt::NoBrush);
        p.drawEllipse(r);

        QString str = QString::number(n.getValue());
        p.setPen(QPen(Qt::white));
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


void MainWindow::onMaxFlowButtonClicked() {

    if (!m_isSelectingSourceOrSink) {
        m_isSelectingSourceOrSink = true;
        qDebug() << "Selectati nodul sursa si destinatie.";

        QMessageBox::information(this, "Selecție noduri", "Alegeți un nod ca sursă și unul ca destinație făcând clic pe noduri.");
    } else {

        if (selectedSource == -1 || selectedSink == -1) {
            qDebug() << "Trebuie să selectezi atât nodul sursă cât și destinația!";
            return;
        }


        int maxFlow = m_graph.fordFulkerson(selectedSource - 1, selectedSink - 1);
        qDebug() << "Fluxul maxim este: " << maxFlow;

        QLabel* resultLabel = new QLabel(this);
        resultLabel->setText("Fluxul Maxim: " + QString::number(maxFlow));
        resultLabel->setGeometry(50, 200, 200, 40);
        resultLabel->show();

        m_isSelectingSourceOrSink = false;
        selectedSource = -1;
        selectedSink = -1;

        m_graph.findMinCut(selectedSource - 1);
        update();
    }
}

void MainWindow::onRedrawGraphButtonClicked() {

    std::vector<std::vector<int>> residualGraph = m_graph.getresidualGraph();

    m_graph.getEdges().clear();

    std::vector<Node> nodes = m_graph.getNodes();

    std::ofstream g("residual.txt");

    for (size_t i = 0; i < residualGraph.size(); ++i) {
        for (size_t j = 0; j < residualGraph[i].size(); ++j) {
            if (residualGraph[i][j] != 0) {
                Node nodeI = nodes[i];
                Node nodeJ = nodes[j];

                m_graph.addEdge(nodeI, nodeJ, residualGraph[i][j]);
                 g << "Adăugat muchie între nodul " << i+1 << " si nodul " << j+1 << " cu costul: " << residualGraph[i][j]<<"\n";
            }
        }
    }

    update();
}





