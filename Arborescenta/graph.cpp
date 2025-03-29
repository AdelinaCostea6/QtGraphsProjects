#include "graph.h"
#include <fstream>
#include <QFile>
#include <QTextStream>
#include <QDebug>
#include <iostream>
#include <string>
#include <stack>

Graph::Graph()
{
    isOriented=false;
    m_matrix.resize(0);

}

void Graph::addNode(const QPoint& p)
{
    Node n;
    n.setPos(p);
    n.setValue(m_nodes.size() + 1);

    bool isOverlapped = false;
    for (auto& existingNode : m_nodes)
    {
        if (((existingNode.getPos().x() - p.x()) * (existingNode.getPos().x() - p.x()) +
             (existingNode.getPos().y() - p.y()) * (existingNode.getPos().y() - p.y())) <= 400)
        {
            isOverlapped = true;
            break;
        }
    }

    if (!isOverlapped)
    {
        m_nodes.push_back(n);

        int newNodeValue = n.getValue();
        m_list[newNodeValue] = std::set<int>();

        int newSize = m_nodes.size();
        m_matrix.resize(newSize);
        for (auto& row : m_matrix)
        {
            row.resize(newSize, 0);
        }

        saveAdjacencyMatrix("adjacency_matrix.txt");
    }
    else
    {
        qDebug() << "There is a risk of overlapping\n";
    }
}
void Graph::addEdge(Node f, Node s)
{
    if (f.getValue() == s.getValue())
    {
        qDebug() << "Node first is same as the second one";
        return;
    }

    int index1 = f.getValue() - 1;
    int index2 = s.getValue() - 1;

    if (m_matrix[index1][index2] == 1)
    {
        qDebug() << "Multigraph: edge already exists between nodes";
        return;
    }

    qDebug() << "Adding edge between:" << f.getValue() << "and" << s.getValue();
    Edge e(f, s);
    m_edges.push_back(e);

    m_matrix[index1][index2] = 1;
    if (!isOriented)
    {
        m_matrix[index2][index1] = 1;
    }

    int firstValue = f.getValue();
    int secondValue = s.getValue();

    m_list[firstValue].insert(secondValue);
    if (!isOriented)
    {
        m_list[secondValue].insert(firstValue);
    }

    saveAdjacencyList("adjacency_list.txt");
    saveAdjacencyMatrix("adjacency_matrix.txt");
}
std::vector<Node> &Graph::getNodes()
{
    return m_nodes;
}

std::vector<Edge> &Graph::getEdges()
{
    return m_edges;
}

void Graph::setOriented(bool oriented)
{
    isOriented=oriented;
}

void Graph::clearAll()
{
    m_nodes.clear();
    m_edges.clear();

    for(auto & row:m_matrix)
    {
        std::fill(row.begin(), row.end(), 0);
    }
    m_matrix.clear();

    for(auto& elem: m_list)
    {
        elem.second.clear();
    }
    m_list.clear();
}

bool Graph::isOrient()
{
    return isOriented;
}

void Graph::saveAdjacencyMatrix(const std::string& filename)
{

    std::ofstream f(filename);
    if (!f)
    {
        qDebug() << "Error: Can not open file " << filename << "\n";
        return;
    }

    f << m_matrix.size() << "\n";

    for (const auto& row : m_matrix)
    {
        for (const auto& value : row)
        {
            f << value << " ";
        }
        f << "\n";
    }

    f.close();
}

void Graph::saveAdjacencyList(const std::string& filename)
{
    std::ofstream f(filename);
    if (!f)
    {
        qDebug() << "Error: Can not open file " << filename << "\n";
        return;
    }

    f<< m_nodes.size() << "\n";
    for (const auto& node : m_list)
    {
        f << node.first << " : ";
        for (const auto& second_node : node.second)
            f << second_node << " ";
        f << "\n";
    }

    f.close();
}

bool Graph::isTree() {

    if (m_edges.size() != m_nodes.size() - 1) {
        return false;
    }


    int startNode = m_nodes[0].getValue();

    if(!isOrient())
    {
       std::set<int> visited;
      depthFirstSearch(startNode, visited);

       if (visited.size() != m_nodes.size()) {
        return false;
       }
    }
   else
    {

    /*Graph transposedGraph = getTransposedGraph();

    std::set<int> visitedTransposed;
    transposedGraph.depthFirstSearch(startNode, visitedTransposed);

    if (visitedTransposed.size() != m_nodes.size()) {
        return false;
    }*/

     std::set<int> visitedOrient;
    bool ok=false;
   for ( Node& node : m_nodes) {
       std::set<int> visitedOrient;
       depthFirstSearch(node.getValue(),visitedOrient);
       if(visitedOrient.size()==m_nodes.size()) ok=true;
    }
   if(ok==false) return false;

    }
    return true;
}


Graph Graph::getTransposedGraph() {
    Graph transposedGraph;

    for (const Node& node : m_nodes) {
        transposedGraph.addNode(node.getPos());
    }


    for (const Edge& edge : m_edges) {
        Node fromNode = edge.getFirst();
        Node toNode = edge.getSecond();
        transposedGraph.addEdge(toNode, fromNode);
    }

    return transposedGraph;
}

void Graph::depthFirstSearch(int startNodeValue, std::set<int>& visited) {
    std::stack<int> V;
    V.push(startNodeValue);
    visited.insert(startNodeValue);

    while (!V.empty()) {
        int currentNode = V.top();
        V.pop();


        for (int neighbor : m_list[currentNode]) {
            if (visited.find(neighbor) == visited.end()) {
                visited.insert(neighbor);
                V.push(neighbor);
            }
        }
    }
}

int Graph::findRoot() {

    if (!isTree()) {
        std::cout << "Graful are cicluri sau nu este conectat, deci nu poate avea o rădăcină unică.\n";
        return -1;
    }

    std::set<int> vis;
    for (const Node& node : m_nodes) {
        vis.insert(node.getValue());
    }


    int root = -1;

    for (const Node& node : m_nodes) {
        int nodeValue = node.getValue();

        std::set<int> tempVis;

        depthFirstSearch(nodeValue, tempVis);

        if (tempVis.size() == m_nodes.size()) {
            root = nodeValue;
            break;
        }
    }

    if (root == -1) {
        std::cout << "Nu exista o radacina valida in graf.\n";
        return -1;
    }


    Graph transposedGraph = getTransposedGraph();
    std::set<int> visitedTransposed;
    transposedGraph.depthFirstSearch(root, visitedTransposed);

    if (visitedTransposed.size() != m_nodes.size()) {
        std::cout << "Nu exista o radacina valida in graf.\n";
        return -1;
    }


    std::cout << "Radacina arborelui este nodul: " << root << "\n";
    return root;
}
