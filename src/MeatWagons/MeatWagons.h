#pragma once
#ifndef MEAT_WAGONS_MEATWAGONS_H
#define MEAT_WAGONS_MEATWAGONS_H

#include <set>
#include "../Graph/Reader.h"
#include "../GraphViewer/GraphVisualizer.h"
#include "Wagon.h"

class MeatWagonsException : public std::exception {
public:
    explicit MeatWagonsException(string  msg) : msg_(std::move(msg)) {}

    [[nodiscard]] string getMessage() const {return(msg_);}
private:
    string msg_;
};

class MeatWagons {
    private:
        int central;
        Graph *graph;
        GraphVisualizer *viewer = new GraphVisualizer(600, 600);
        unordered_map<int, Vertex*> pointsOfInterest;

        set<Wagon*> wagons;
        vector<Request> requests;
        int zoneMaxDist;

    public:
        MeatWagons(const int wagons, const int maxDist) {
            for(int i = 0; i < wagons; i++) this->wagons.insert(new Wagon(i, 1));
            this->zoneMaxDist = maxDist;
        }

        int getCentral() const;
        void setCentral(const int &id);

        Graph *const getGraph() const;
        void setGraph(Graph *graph);
        void setGraph(const string path);
        void showGraph() const;

        void preProcess(int node) const;

        void shortestPath(int option, int origin, int dest);

        void readRequests() const;
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

void MeatWagons::setGraph(Graph *graph) {
    if(graph == nullptr) throw MeatWagonsException("Graph is null");
    this->graph = graph;
}

void MeatWagons::setGraph(const string graphPath) {
    Reader graphReader = Reader(graphPath);
    Graph* graphRead = new Graph();
    graphReader.readGraph(graphRead, central, pointsOfInterest);

    if(graphRead == nullptr) throw MeatWagonsException("Graph is null");

    this->graph = graphRead;
}

void MeatWagons::showGraph() const{
    this->viewer->draw(graph);
}

void MeatWagons::preProcess(int node) const {
    if(this->graph == nullptr) throw MeatWagonsException("Graph is null");
    if(!this->graph->preProcess(node)) throw MeatWagonsException("Vertex does not exist");
    showGraph();
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
    showGraph();
}

void MeatWagons::readRequests() const {

}

#endif //MEAT_WAGONS_MEATWAGONS_H