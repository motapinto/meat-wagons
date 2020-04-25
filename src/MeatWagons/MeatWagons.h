#pragma once
#ifndef MEAT_WAGONS_MEATWAGONS_H
#define MEAT_WAGONS_MEATWAGONS_H

#include "Wagon.h"
#include "../GraphViewer/GraphVisualizer.h"

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

    public:
        const int getCentral() const;
        const void setCentral(const int &id);

        Graph *const getGraph() const;
        const void setGraph(Graph *graph);
        const void setGraph(string path);
        const void showGraph() const;

        const void preProcess(int node) const;

        void shortestPath(int option, int origin, int dest);
};

const int MeatWagons::getCentral() const {
    return this->central;
}

const void MeatWagons::setCentral(const int &id) {
    this->central = id;
}

Graph *const MeatWagons::getGraph() const {
    return this->graph;
}

const void MeatWagons::setGraph(Graph *graph) {
    if(graph == nullptr) throw MeatWagonsException("Graph is null");
    this->graph = graph;
}

const void MeatWagons::setGraph(string graphPath) {
    Reader graphReader = Reader(graphPath, central);
    Graph* graphRead = graphReader.read();

    if(graphRead == nullptr)throw MeatWagonsException("Graph is null");

    this->graph = graphRead;
    this->central = graphReader.getCentral();
}

const void MeatWagons::showGraph() const{
    viewer->draw(graph);
}

const void MeatWagons::preProcess(int node) const {
    if(graph == nullptr) throw MeatWagonsException("Graph is null");
    if(!graph->preProcess(node)) throw MeatWagonsException("Vertex does not exist");
    showGraph();
}

void MeatWagons::shortestPath(int option, int origin, int dest) {
    if(graph == nullptr) throw MeatWagonsException("Graph is null");

    switch (option) {
        case 1: if (!graph->dijkstra(origin, dest)) throw MeatWagonsException("Vertex not found"); break;
        case 2: if (!graph->dijkstraOrientedSearch(origin, dest)) throw MeatWagonsException("Vertex was not found");break;
        case 3: if (!graph->dijkstraBidirectional(origin, dest)) throw MeatWagonsException("Vertex was not found");break;
    }

    vector<int> vert, edges;
    graph->getPathTo(dest, vert, edges);
    viewer->setPath(vert, edges);
    showGraph();
}

#endif //MEAT_WAGONS_MEATWAGONS_H