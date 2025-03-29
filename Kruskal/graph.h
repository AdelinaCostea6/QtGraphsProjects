#ifndef GRAPH_H
#define GRAPH_H

#include <string>
#include "edge.h"
#include <vector>
#include <map>
#include <set>

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
    std::vector<Edge>kruskal();
    void setMstEdgesGeneric(const std::vector<Edge>& edges);
    std::vector<Edge>& getMstEdgesGeneric();
    std::vector<Edge>kruskalMST();
    bool isOriented;
    bool isConnected();

private:
    std::vector<Node>m_nodes;
    std::vector<Edge>m_edges;
    std::vector<std::vector<int>>m_matrix;
    std::map<int,std::set<int>>m_list;
    std::vector<Edge> m_mstEdgesGeneric;



};

#endif // GRAPH_H
