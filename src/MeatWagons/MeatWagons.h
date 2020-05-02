#pragma once
#ifndef MEAT_WAGONS_MEATWAGONS_H
#define MEAT_WAGONS_MEATWAGONS_H

#include <set>
#include "Request.h"
#include "../Graph/Reader.h"
#include "../GraphViewer/GraphVisualizer.h"
#include "Wagon.h"

class MeatWagonsException : public std::exception {
public:
    MeatWagonsException(string  msg) : msg_(std::move(msg)) {}
    string getMessage() const {return(msg_);}
    
private:
    string msg_;
};

class MeatWagons {
    private:
        int central;
        Graph *graph;
        GraphVisualizer *viewer = new GraphVisualizer(600, 600);
        unordered_map<int, Vertex*> pointsOfInterest;
        set<Wagon> wagons;
        multiset<Request> requests;
        int zoneMaxDist;

    public:
        MeatWagons(const int wagons, const int maxDist) {
            for(int i = 0; i < wagons; i++) this->wagons.insert(Wagon(i, 1));
            this->zoneMaxDist = maxDist;
        }

        Graph *const getGraph() const;
        int getCentral() const;

        void setGraph(const string path);
        void setCentral(const int &id);

        void showGraph() const;
        void preProcess(int node);
        void shortestPath(int option, int origin, int dest);
        void readRequests(string requestsPath);
};

int MeatWagons::getCentral() const {
    return this->central;
}

void MeatWagons::setCentral(const int &id) {
    this->central = id;
}

Graph *const MeatWagons::getGraph() const {
    return this->graph;
}


void MeatWagons::setGraph(const string graphPath) {
    Reader graphReader = Reader(graphPath);
    Graph* graphRead = new Graph();

    if(!graphReader.readGraph(graphRead, central, pointsOfInterest)) throw MeatWagonsException("Graph is null");
    readRequests(graphPath);

    this->graph = graphRead;
}

void MeatWagons::showGraph() const {
    this->viewer->draw(this->graph);
}

void MeatWagons::preProcess(int node) {
    if(this->graph == nullptr) throw MeatWagonsException("Graph is null");
    if(!this->graph->preProcess(node)) throw MeatWagonsException("Vertex does not exist");

    for(auto it = this->requests.begin(); it != this->requests.end(); it++) {
        if(this->graph->findVertex((*it).getDest()) == nullptr)
            this->requests.erase(*it);
    }
}

void MeatWagons::shortestPath(int option, int origin, int dest) {
    if(this->graph == nullptr) throw MeatWagonsException("Graph is null");

    switch (option) {
        case 1: if (!this->graph->dijkstra(origin, dest)) throw MeatWagonsException("Vertex not found"); break;
        case 2: if (!this->graph->dijkstraOrientedSearch(origin, dest)) throw MeatWagonsException("Vertex was not found");break;
        case 3: if (!this->graph->dijkstraBidirectional(origin, dest)) throw MeatWagonsException("Vertex was not found");break;
    }

    vector<int> vert, edges;
    this->graph->getPathTo(dest, vert, edges);
    viewer->setPath(vert, edges);
}

void MeatWagons::readRequests(string requestsPath) {
    Reader graphReader = Reader(requestsPath);

    if(!graphReader.readRequests(requests)) throw MeatWagonsException("Graph is null");
}

#endif //MEAT_WAGONS_MEATWAGONS_H