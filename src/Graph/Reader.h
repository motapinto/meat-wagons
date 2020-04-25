#pragma once
#ifndef MEAT_WAGONS_READER_H
#define MEAT_WAGONS_READER_H

#include <fstream>
#include "Graph.h"

using namespace std;

class Reader {
    private:
        string path;
        int central;
        unordered_map<int, Vertex*> pointsOfInterest;

    public:
        explicit Reader(string &path, int &central, unordered_map<int, Vertex*> &pointsOfInterest) {
            this->path = path;
            this->central = central;
            this->pointsOfInterest = pointsOfInterest;
        }

        [[nodiscard]] Graph * read();
        bool setTags(Graph &graph);
        const int getCentral() const;
        bool setCentral(Graph &graph);
};

Graph * Reader::read() {
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
        graph.addVertex(id, x, y);
        x > maxX ? maxX = x : maxX;
        y > maxY ? maxY = y : maxY;
        x < minX ? minX = x : minX;
        y < minY ? minY = y : minY;
    }

    graph.setWidth(maxX - minX);
    graph.setHeight(maxY - minY);
    graph.setOffsetX(1.01*minX);
    graph.setOffsetY(1.0005*minY);

    edgesStream >> numEdges;
    for (int i = 1; i <= numEdges; i++) {
        edgesStream >> c >> origin >> c >> dest >> c;
        graph.addEdge(i, origin, dest);
    }

    nodesStream.close();
    edgesStream.close();

    setTags(graph);
    setCentral(graph);

    return &graph;
}

bool Reader::setTags(Graph &graph) {
    ifstream tagsStream(path + "/tags.txt");

    if(!tagsStream.is_open()) return false;

    int id, trash, numTags;
    string tagName;

    tagsStream >> trash;
    tagsStream >> tagName >> numTags;
    for(int j = 0; j < numTags; j++) {
        tagsStream >> id;
        Vertex *vertex = graph.findVertex(id);
        if(vertex == nullptr) return false;
        vertex->setTag(Vertex::INTEREST_POINT);
        pointsOfInterest.insert(pair<int, Vertex*>(vertex->getId(), vertex));
    }
    return true;
}

const int Reader::getCentral() const {
    return this->central;
}

bool Reader::setCentral(Graph &graph) {
    int pos = path.find_last_of('/');
    string city = path.substr(pos + 1);

    if(city == "Porto") {
        graph.findVertex(90379359)->setTag(Vertex::CENTRAL);
        this->central = 90379359;
    }
    return true;
}

#endif //MEAT_WAGONS_READER_H