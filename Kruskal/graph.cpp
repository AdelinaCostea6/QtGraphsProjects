#include "graph.h"
#include <fstream>
#include <QFile>
#include <QTextStream>
#include <QDebug>
#include <iostream>
#include <string>
#include <stack>
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

std::vector<Edge> Graph::kruskalMST() {
    std::vector<Edge> mst;
    std::vector<Edge> edges = m_edges;

    std::sort(edges.begin(), edges.end(), [](const Edge& a, const Edge& b) {
        return a.getCost() < b.getCost();
    });

    std::vector<int> parent(m_nodes.size());
    for (int i = 0; i < m_nodes.size(); ++i) {
        parent[i] = i;
    }

    auto find = [&](int node) {
        while (node != parent[node]) {
            parent[node] = parent[parent[node]];
            node = parent[node];
        }
        return node;
    };

    auto unite = [&](int u, int v) {
        parent[find(u)] = find(v);
    };

    for (const auto& edge : edges) {
        int u = edge.getFirst().getValue() - 1;
        int v = edge.getSecond().getValue() - 1;

        if (find(u) != find(v)) {
            mst.push_back(edge);
            unite(u, v);
        }

        if (mst.size() == m_nodes.size() - 1) {
            break;
        }
    }

    return mst;
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

void Graph::setMstEdgesGeneric(const std::vector<Edge>& edges) {
    m_mstEdgesGeneric = edges;
}


std::vector<Edge>& Graph::getMstEdgesGeneric() {
    m_mstEdgesGeneric=kruskalMST();
    return m_mstEdgesGeneric;
}
