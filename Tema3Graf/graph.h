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
    void depthFirstSearch(int startNodeValue, const std::string& filename);
    bool hasCycle();
    bool topologicalSortToFile(const std::string& filename);
    const std::vector<int>& getTopologicalOrder() const;
    const std::map<int,std::set<int>>& getAdjList() const;




private:
    std::vector<Node>m_nodes;
    std::vector<Edge>m_edges;
    std::vector<std::vector<int>>m_matrix;
    std::map<int,std::set<int>>m_list;
    bool isOriented;
    std::vector<int>m_topologicalOrder;



};

#endif // GRAPH_H
