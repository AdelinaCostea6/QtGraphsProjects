#ifndef GRAPH_H
#define GRAPH_H

#include <string>
#include "edge.h"
#include <vector>
#include <map>
#include <set>
#include <qDebug>
#include <queue>
#include <fstream>
class Graph
{
public:
    Graph();
    void addNode(const QPoint& p);
    void addEdge(Node f, Node s, int cost);
    std::vector<Node>&getNodes();
    std::vector<Edge>&getEdges();
    void saveAdjacencyMatrix(const std::string& filename);
    void saveAdjacencyList(const std::string& filename);
    void setOriented(bool oriented);
    void clearAll();
    bool isOrient();
    bool isOriented;
    bool isConnected();
    int fordFulkerson(int s, int t);
    void saveAdjacencyMatrixFlux(const std::string& filename);
    const std::vector<Edge>& getMinCutEdges() const { return m_minCutEdges; }
    void findMinCut(int s);
    std::vector<std::vector<int>>m_matrix1;
    std::vector<std::vector<int>> getresidualGraph();
    void saveAdjacencyMatrixFluxUpdated(const std::string& filename);



private:
    std::vector<Node>m_nodes;
    std::vector<Edge>m_edges;
    std::map<int,std::set<int>>m_list;
    std::vector<std::vector<int>> residualGraph;
    std::vector<std::vector<int>>m_matrix;
    std::vector<Edge> m_minCutEdges;
    bool bfs(int s, int t, std::vector<int>& parent);



};

#endif // GRAPH_H
