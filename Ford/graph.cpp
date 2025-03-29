#include "graph.h"
Graph::Graph()
{
    isOriented=false;
    m_matrix.resize(0);
    m_matrix1.resize(0);

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
        m_matrix1.resize(newSize);
        for (auto& row : m_matrix)
        {
            row.resize(newSize, 0);
        }
        for (auto& row : m_matrix1)
        {
            row.resize(newSize, 0);
        }

        saveAdjacencyMatrix("adjacency_matrix.txt");
        saveAdjacencyMatrixFlux("adjacency_matrixFlux.txt");
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

    /*if (m_matrix[index1][index2] == 1)
    {
        qDebug() << "Multigraph: edge already exists between nodes";
        return;
    }*/

    qDebug() << "Adding edge between:" << f.getValue() << "and" << s.getValue();
    Edge e(f, s, cost);
    m_edges.push_back(e);

    // Setează costul (capacitatea) în matricea de costuri m_matrix1
    m_matrix1[index1][index2] = cost;

    // În m_matrix, păstrezi doar informația că există o muchie (fără costuri)
    m_matrix[index1][index2] = 1;

    if (!isOriented)
    {
        m_matrix[index2][index1] = 1;
        m_matrix1[index2][index1] = cost;  // Setează costul și în sens invers pentru graf neorientat
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
    saveAdjacencyMatrixFlux("adjacency_matrixFlux.txt");  // Salvează corect matricea de costuri
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

void Graph::saveAdjacencyMatrixFlux(const std::string& filename)
{

    std::ofstream f(filename);
    if (!f)
    {
        qDebug() << "Error: Can not open file " << filename << "\n";
        return;
    }

    f << m_matrix1.size() << "\n";

    for (const auto& row : m_matrix1)
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


void Graph::saveAdjacencyMatrixFluxUpdated(const std::string& filename)
{

    std::ofstream f(filename);
    if (!f)
    {
        qDebug() << "Error: Can not open file " << filename << "\n";
        return;
    }

    std::vector<std::vector<int>>residual=getresidualGraph();

    f << m_matrix1.size() << "\n";

    for (const auto& row : residual)
    {
        for (const auto& value : row)
        {
            f << value << " ";
        }
        f << "\n";
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

bool Graph::bfs(int s, int t, std::vector<int>& parent) {
    std::vector<bool> visited(m_nodes.size(), false);
    std::queue<int> queue;

    queue.push(s);
    visited[s] = true;

    while (!queue.empty()) {
        int u = queue.front();
        queue.pop();

        for (int v = 0; v < m_nodes.size(); ++v) {
            if (!visited[v] && residualGraph[u][v] > 0) {
                queue.push(v);
                visited[v] = true;
                parent[v] = u;
                if (v == t) return true;
            }
        }
    }

    return false;
}

int Graph::fordFulkerson(int s, int t) {
    residualGraph = m_matrix1;
    int maxFlow = 0;
    std::vector<int> parent(m_nodes.size(), -1);

    while (bfs(s, t, parent)) {

        bfs(s, t, parent);

        int pathFlow = INT_MAX;
        for (int v = t; v != s; v = parent[v]) {
            int u = parent[v];
            pathFlow = std::min(pathFlow, residualGraph[u][v]);
        }

        for (int v = t; v != s; v = parent[v]) {
            int u = parent[v];
            residualGraph[u][v] -= pathFlow;
            residualGraph[v][u] += pathFlow;
        }

        maxFlow += pathFlow;
    }

    findMinCut(s);

    saveAdjacencyMatrixFluxUpdated("updatedMatrix.txt");

    return maxFlow;
}

void Graph::findMinCut(int s) {
    std::vector<bool> visited(m_nodes.size(), false);

    std::queue<int> queue;
    queue.push(s);
    visited[s] = true;

    while (!queue.empty()) {
        int u = queue.front();
        queue.pop();

        for (int v = 0; v < m_nodes.size(); ++v) {
            if (!visited[v] && residualGraph[u][v] > 0) {
                visited[v] = true;
                queue.push(v);
            }
        }
    }

    m_minCutEdges.clear();
    for (const auto& edge : m_edges) {
        int u = edge.getFirst().getValue() - 1;
        int v = edge.getSecond().getValue() - 1;
        if (visited[u] && !visited[v]) {
            m_minCutEdges.push_back(edge);
        }
    }
}

std::vector<std::vector<int> > Graph::getresidualGraph()
{
    return residualGraph;
}



