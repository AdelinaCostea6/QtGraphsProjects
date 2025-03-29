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
    void addEdge(Node f, Node s);
    std::vector<Node>&getNodes();
    std::vector<Edge>&getEdges();
    void saveAdjacencyMatrix(const std::string& filename);
    void saveAdjacencyList(const std::string& filename);
    void setOriented(bool oriented);
    void clearAll();
    bool isOrient();
    bool isTree();
    Graph getTransposedGraph();
    void depthFirstSearch(int startNodeValue, std::set<int>& visited);
    bool checkFullReachability(int root, std::vector<bool>& visited);
    int findRoot();



private:
    std::vector<Node>m_nodes;
    std::vector<Edge>m_edges;
    std::vector<std::vector<int>>m_matrix;
    std::map<int,std::set<int>>m_list;
    bool isOriented;



};

#endif // GRAPH_H
