#pragma once
#ifndef MEAT_WAGONS_READER_H
#define MEAT_WAGONS_READER_H

#include <fstream>
#include "Graph.h"

using namespace std;

class Reader {
    private:
        string path;
    public:
        explicit Reader(string &path) {
            this->path = path;
        }

        [[nodiscard]] Graph* read() const;
};

Graph* Reader::read() const {
    ifstream nodesStream(path + "/nodes.txt");
    ifstream edgesStream(path + "/edges.txt");

    if(!nodesStream.is_open() || !edgesStream.is_open())
        return nullptr;

    static Graph graph = Graph();

    int id, origin, dest;
    int numNodes, numEdges;
    double x, y;
    char c;

    int minX = numeric_limits<int>::max(), minY = numeric_limits<int>::max(), maxX = 0, maxY = 0;

    nodesStream >> numNodes;
    for (int i = 1; i <= numNodes; i++) {
        nodesStream >> c >> id >> c >> x >> c >> y >> c;
        graph.addVertex(id, x, y); //NAO ESQUECER INVERTER O GRAFO (CIDADE DO PORTO ESTTA AO CNTRARIO)
        x > maxX ? maxX = x : maxX;
        y > maxY ? maxY = y : maxY;
        x < minX ? minX = x : minX;
        y < minY ? minY = y : minY;
    }

    graph.setWidth(maxX - minX);
    graph.setHeight(maxY - minY);
    graph.setOffsetX(1.01*minX);
    cout << minY << endl;
    graph.setOffsetY(1.0005*minY);

    edgesStream >> numEdges;
    for (int i = 1; i <= numEdges; i++) {
        edgesStream >> c >> origin >> c >> dest >> c;
        graph.addEdge(i, origin, dest);
    }

    nodesStream.close();
    edgesStream.close();

    return &graph;
}

#endif //MEAT_WAGONS_READER_H