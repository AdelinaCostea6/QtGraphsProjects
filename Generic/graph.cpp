#include "graph.h"
#include <fstream>
#include <QFile>
#include <QTextStream>
#include <QDebug>
#include <iostream>
#include <string>
#include <queue>

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
void Graph::addEdge(Node f, Node s, int cost)
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
    Edge e(f, s, cost);
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

    f << m_nodes.size() << "\n";
    for (const auto& edge : m_edges)
    {
        f << edge.getFirst().getValue() << " -> " << edge.getSecond().getValue()
        << " [Cost: " << edge.getCost() << "]\n";
    }

    f.close();
}
bool Graph::isConnected() {
    if (m_nodes.empty()) return true;

    std::vector<bool> visited(m_nodes.size(), false);
    std::queue<int> queue;
    queue.push(1);
    visited[0] = true;

    while (!queue.empty()) {
        int current = queue.front();
        queue.pop();

        for (const auto& neighbor : m_list[current]) {
            if (!visited[neighbor - 1]) {
                visited[neighbor - 1] = true;
                queue.push(neighbor);
            }
        }
    }

    return std::all_of(visited.begin(), visited.end(), [](bool v) { return v; });
}
std::vector<Edge> Graph::minimumSpanningTree() {

    int n = m_nodes.size();
    std::vector<std::set<int>> Ni(n);
    std::vector<Edge> Ai;


    for (int i = 0; i < n; i++) {
        Ni[i].insert(i);
    }

    std::set<std::pair<int, int>> addedEdges;

    while (Ai.size() < n - 1) {
        int minCost = INT_MAX;
        Edge minEdge(Node(), Node(), 0);
        int uSetIndex = -1, vSetIndex = -1;

        for (const auto& edge : m_edges) {
            int u = edge.getFirst().getValue() - 1;
            int v = edge.getSecond().getValue() - 1;
            if (Ni[u] != Ni[v] && addedEdges.find({std::min(u, v), std::max(u, v)}) == addedEdges.end()) {
                if (edge.getCost() < minCost) {
                    minCost = edge.getCost();
                    minEdge = edge;
                    uSetIndex = u;
                    vSetIndex = v;
                }
            }
        }

        if (uSetIndex != -1 && vSetIndex != -1) {
            Ai.push_back(minEdge);
            qDebug() << "Added edge: " << minEdge.getFirst().getValue() << " - " << minEdge.getSecond().getValue();
            addedEdges.insert({std::min(uSetIndex, vSetIndex), std::max(uSetIndex, vSetIndex)});
        }

        for (int i = 0; i < n; i++) {
            if (Ni[i] == Ni[vSetIndex]) {
                Ni[uSetIndex].insert(i);
            }
        }

        Ni[vSetIndex].clear();
    }

    return Ai;
}
void Graph::setMstEdgesGeneric(const std::vector<Edge>& edges) {
    m_mstEdgesGeneric = edges;
}

std::vector<Edge>& Graph::getMstEdgesGeneric() {
    return m_mstEdgesGeneric;
}
