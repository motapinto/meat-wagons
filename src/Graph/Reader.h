#pragma once
#ifndef MEAT_WAGONS_READER_H
#define MEAT_WAGONS_READER_H

#include <fstream>
#include "Graph.h"

class Reader {
    private:
        string path;

    public:
        Reader(const string &path) : path(path) {}

        bool readGraph(Graph *graph, int &central, unordered_map<int, Vertex*> &pointsOfInterest);
        bool readRequests(multiset<Request> &requestVector);
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


bool Reader::readRequests(multiset<Request> &requestVector) {
    ifstream requests(path + "/requests.txt");

    if(!requests.is_open()) return false;

    string name;
    int dest, priority;
    int hour, min, sec;

    while(requests >> name) {
        requests >> dest >> priority >> hour >> min >> sec;
        Time arrival(hour, min, sec);
        Request request = Request(name, dest, priority, arrival);
        requestVector.insert(request);
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
        graph->findVertex(126401360)->setTag(Vertex::CENTRAL);
        central = 126401360;
    }
    else if(city == "Aveiro") {
        graph->findVertex(503706165)->setTag(Vertex::CENTRAL);
        central = 503706165;
    }
    else if(city == "Braga") {
        graph->findVertex(211309060)->setTag(Vertex::CENTRAL);
        central = 211309060;
    }
    else if(city == "Coimbra") {
        graph->findVertex(101591500)->setTag(Vertex::CENTRAL);
        central = 101591500;
    }
    else if(city == "Ermesinde") {
        graph->findVertex(445088873)->setTag(Vertex::CENTRAL);
        central = 445088873;
    }
    else if(city == "Fafe") {
        graph->findVertex(26130553)->setTag(Vertex::CENTRAL);
        central = 26130553;
    }
    else if(city == "Gondomar") {
        graph->findVertex(444058628)->setTag(Vertex::CENTRAL);
        central = 444058628;
    }
    // else if(city == "Lisboa") {
    //     graph->findVertex()->setTag(Vertex::CENTRAL);
    //     central = ;
    // }
    else if(city == "Maia") {
        graph->findVertex(492329895)->setTag(Vertex::CENTRAL);
        central = 492329895;
    }
    else if(city == "Viseu") {
        graph->findVertex(603955824)->setTag(Vertex::CENTRAL);
        central = 603955824;
    }
    // else if(city == "Portugal") {
    //     graph->findVertex()->setTag(Vertex::CENTRAL);
    //     central = ;
    // }

    return true;
}

#endif //MEAT_WAGONS_READER_H