#pragma once
#ifndef MEAT_WAGONS_READER_H
#define MEAT_WAGONS_READER_H

#include <string>
#include <vector>
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
    ifstream nodesStream(this->path + "/nodes_x_y.txt");
    ifstream edgesStream(this->path + "edges.txt");

    if(!nodesStream.is_open() || edgesStream.is_open())
        return nullptr;

    static Graph graph = Graph();

    int id, origin, dest;
    int numNodes, numEdges;
    double x, y;
    char c;

    nodesStream >> numNodes;
    for (int i = 0; i < numNodes; i++) {
        nodesStream >> c >> id >> c >> x >> c >> y >> c;
        graph.addVertex(i, x, y);
    }

    edgesStream >> numEdges;
    for (int i = 0; i < numEdges; i++) {
        edgesStream >> c >> origin >> c >> dest >> c;
        graph.addEdge(i, origin, dest);
    }

    nodesStream.close();
    edgesStream.close();

    return &graph;
}

#endif //MEAT_WAGONS_READER_H