#pragma once
#ifndef MEAT_WAGONS_READER_H
#define MEAT_WAGONS_READER_H

#include <fstream>
#include "Graph.h"
#include "../MeatWagons/Request.h"

using namespace std;

class Reader {
    private:
        string path;

    public:
        Reader(const string &path) : path(path) {}

        bool readGraph(Graph *graph, int &central, unordered_map<int, Vertex*> &pointsOfInterest);
        bool readRequests(vector<Request> &requestVector);
        bool setTags(Graph *graph, unordered_map<int, Vertex*> &pointsOfInterest);
        bool setCentral(Graph *graph, int &central);
};

bool Reader::readGraph(Graph *graph, int &central, unordered_map<int, Vertex*> &pointsOfInterest) {
    ifstream nodesStream(path + "/nodes.txt");
    ifstream edgesStream(path + "/edges.txt");

    if(!nodesStream.is_open() || !edgesStream.is_open()) return false;

    int id, origin, dest;
    int numNodes, numEdges;
    double x, y;
    char c;

    int minX = numeric_limits<int>::max(), minY = numeric_limits<int>::max(), maxX = 0, maxY = 0;

    nodesStream >> numNodes;
    for (int i = 1; i <= numNodes; i++) {
        nodesStream >> c >> id >> c >> x >> c >> y >> c;
        graph->addVertex(id, x, y);
        x > maxX ? maxX = x : maxX;
        y > maxY ? maxY = y : maxY;
        x < minX ? minX = x : minX;
        y < minY ? minY = y : minY;
    }

    graph->setOffsetX(1.01*minX);
    graph->setOffsetY(1.0005*minY);

    edgesStream >> numEdges;
    for (int i = 1; i <= numEdges; i++) {
        edgesStream >> c >> origin >> c >> dest >> c;
        graph->addEdge(i, origin, dest);
    }

    nodesStream.close();
    edgesStream.close();

    setTags(graph, pointsOfInterest);
    setCentral(graph, central);

    return true;
}

bool Reader::readRequests(vector<Request> &requestVector) {
    ifstream requests(path + "/requests.txt");

    if(!requests.is_open()) return false;

    string name;
    int dest, priority;
    int hour, min, sec;

    while(requests >> name) {
        requests >> dest >> priority >> hour >> min >> sec;
        Time arrival(hour, min, sec);
        Request request = Request(name, dest, priority, arrival);
        requestVector.push_back(request);
    }
    return true;
}

bool Reader::setTags(Graph *graph, unordered_map<int, Vertex*> &pointsOfInterest) {
    ifstream tagsStream(path + "/tags.txt");

    if(!tagsStream.is_open()) return false;

    int id, trash, numTags;
    string tagName;

    tagsStream >> trash;
    tagsStream >> tagName >> numTags;
    for(int j = 0; j < numTags; j++) {
        tagsStream >> id;
        Vertex *vertex = graph->findVertex(id);
        if(vertex == nullptr) return false;
        vertex->setTag(Vertex::INTEREST_POINT);
        pointsOfInterest.insert(pair<int, Vertex*>(vertex->getId(), vertex));
    }
    return true;
}

bool Reader::setCentral(Graph *graph, int &central) {
    int pos = path.find_last_of('/');
    string city = path.substr(pos + 1);

    if(city == "Porto") {
        graph->findVertex(90379359)->setTag(Vertex::CENTRAL);
        central = 90379359;
    }
    return true;
}

#endif //MEAT_WAGONS_READER_H