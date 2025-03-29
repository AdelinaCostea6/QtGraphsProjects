#include "graph.h"
#include <fstream>
#include <QFile>
#include <QTextStream>
#include <QDebug>
#include <iostream>
#include <string>
#include <stack>
#include <unordered_set>
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

void Graph::depthFirstSearch(int startNodeValue, const std::string& filename)
{
    std::stack<int> V;
    std::set<int> U;
    std::map<int, int> t1;
    std::map<int, int> t2;
    std::map<int, int> p;
    int k = 1;


    for (const Node& node : m_nodes) {
        int nodeValue = node.getValue();
        U.insert(nodeValue);
        t1[nodeValue] = std::numeric_limits<int>::max();
        t2[nodeValue] = std::numeric_limits<int>::max();
        p[nodeValue] = 0;
    }

    V.push(startNodeValue);
    U.erase(startNodeValue);
    t1[startNodeValue] = k++;

    std::ofstream file(filename);
    if (!file.is_open()) {
        qDebug() << "Nu se poate deschide fișierul pentru salvare.";
        return;
    }


    while (!V.empty()) {
        int x = V.top();
        bool foundNeighbor = false;


        for (int y : m_list[x]) {
            if (U.find(y) != U.end()) {
                V.push(y);
                U.erase(y);
                p[y] = x;
                t1[y] = k++;
                foundNeighbor = true;
                break;
            }
        }

        if (!foundNeighbor) {
            V.pop();
            t2[x] = k++;
        }
    }

    file << "Nod\tParinte\tTimp intrare\tTimp ieșire\n";
    for (const auto& node : m_nodes) {
        int nodeValue = node.getValue();
        file << nodeValue << "\t" << p[nodeValue] << "\t" << t1[nodeValue] << "\t" << t2[nodeValue] << "\n";
    }

    file.close();
    qDebug() << "Rezultatele parcurgerii DFS au fost salvate în " << QString::fromStdString(filename);
}


bool Graph::hasCycle() {
    std::vector<int> state(m_nodes.size(), 1);
    for (int i = 0; i < m_nodes.size(); ++i) {
        if (state[i] == 1) {
            std::stack<int> nodeStack;
            nodeStack.push(i);

            while (!nodeStack.empty()) {
                int currentNode = nodeStack.top();


                if (state[currentNode] == 1) {
                    state[currentNode] = 2;
                }

                bool hasUnvisitedNeighbor = false;


                for (int neighbor : m_list[currentNode + 1]) {
                    int neighborIndex = neighbor - 1;

                    if (state[neighborIndex] == 1) {

                        nodeStack.push(neighborIndex);
                        hasUnvisitedNeighbor = true;
                        break;
                    } else if (state[neighborIndex] == 2) {

                        return true;
                    }
                }


                if (!hasUnvisitedNeighbor) {
                    state[currentNode] = 3;
                    nodeStack.pop();
                }
            }
        }
    }

    return false;
}


bool Graph::topologicalSortToFile(const std::string& filename) {
    if (hasCycle())
    {
        std::cout << "Graful contine un circuit. Sortarea topologică nu poate fi realizata." << std::endl;
        return false;
    }


    std::vector<int> t1(m_nodes.size(), std::numeric_limits<int>::max());
    std::vector<int> t2(m_nodes.size(), std::numeric_limits<int>::max());
    std::vector<int> parent(m_nodes.size(), 0);
    std::unordered_set<int> U;
    std::stack<int> W;
    int k = 1;
    std::stack<int> V;


    for (const Node& node : m_nodes) {
        U.insert(node.getValue() - 1);
    }


    while (W.size() < m_nodes.size()) {
        if (V.empty()) {
            if (!U.empty()) {
                int s = *U.begin();
                U.erase(s);
                V.push(s);
                t1[s] = k++;
            }
        }

        while (!V.empty()) {
            int x = V.top();
            bool hasUnvisitedNeighbor = false;


            for (int neighbor : m_list[x + 1]) {
                int neighborIndex = neighbor - 1;

                if (U.find(neighborIndex) != U.end()) {

                    V.push(neighborIndex);
                    U.erase(neighborIndex);
                    parent[neighborIndex] = x + 1;
                    t1[neighborIndex] = k++;
                    hasUnvisitedNeighbor = true;
                    break;
                }
            }

            if (!hasUnvisitedNeighbor) {
                V.pop();
                W.push(x + 1);
                t2[x] = k++;
            }
        }
    }

    m_topologicalOrder.clear();
    std::ofstream outFile(filename);
    if (outFile.is_open()) {
        std::vector<int> topologicalOrder;
        while (!W.empty())
        {
            topologicalOrder.push_back(W.top());
            W.pop();
        }


        std::reverse(topologicalOrder.begin(), topologicalOrder.end());
        for (auto it = topologicalOrder.rbegin(); it != topologicalOrder.rend(); ++it) {
            outFile << *it << " ";
            m_topologicalOrder.push_back(*it);
        }

        outFile.close();
        std::cout << "Sortarea topologica a fost scrisa în fisierul " << filename << "." << std::endl;
    } else {
        std::cerr << "Eroare la deschiderea fisierului pentru scriere." << std::endl;
        return false;
    }

    return true;
}

const std::vector<int> &Graph::getTopologicalOrder() const
{
    return m_topologicalOrder;
}

const std::map<int, std::set<int> > &Graph::getAdjList() const
{
    return m_list;
}


