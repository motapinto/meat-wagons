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

    nodesStream >> numNodes;
    for (int i = 1; i <= numNodes; i++) {
        nodesStream >> c >> id >> c >> x >> c >> y >> c;
        graph.addVertex(id, x, y);
    }

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