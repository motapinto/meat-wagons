#pragma once
#ifndef MEAT_WAGONS_GRAPHVISUALIZER_H
#define MEAT_WAGONS_GRAPHVISUALIZER_H

#include <thread>
#include "../Graph/Graph.h"
#include "GraphViewer/cpp/graphviewer.h"

class GraphVisualizer {
private:
    int width, height;
    GraphViewer *gv;

public:
    GraphVisualizer(const int width, const int height) {
        this->width = width;
        this->height = height;
        this->gv = new GraphViewer(1, 1, false);
    }

    ~GraphVisualizer() {
        delete gv;
    }

    GraphViewer* getViewer() const ;
    void drawFromThread(Graph *graph);
    void draw(Graph *graph);
    void setPath(const vector<int> &edges, const string &edgeColor, const bool isShortestPath = false);
    void setNode(const int id, const int size, const string color, const string label);
    void drawShortestPathFromThread(const unordered_set<int> &processedEdges, const unordered_set<int> &processedEdgesInv, const vector<Edge> &edges, Graph *graph);
    void drawShortestPath(const unordered_set<int> &processedEdges, const unordered_set<int> &processedEdgesInv, const vector<Edge> &edges, Graph *graph);
};

GraphViewer* GraphVisualizer::getViewer() const {
    return gv;
}

void GraphVisualizer::drawFromThread(Graph *graph) {
    thread threadProcess(&GraphVisualizer::draw, this, graph);
    threadProcess.detach();
}

// no final tentar fazer animacao como na tp com sleeps
void GraphVisualizer::draw(Graph *graph) {
    this->gv = new GraphViewer(1, 1, false);

    gv->createWindow(width, height);
    //vertexes settings
    gv->defineVertexColor("black");
    //edges settings
    gv->defineEdgeColor("gray");
    gv->defineEdgeCurved(false);

    vector<Vertex*> vertexSet = graph->getVertexSet();

    for(Vertex *origin : vertexSet)
        gv->addNode(origin->getId(), origin->getPosition().getX() - graph->getOffsetX(), origin->getPosition().getY() - graph->getOffsetY());

    for(Vertex *origin : vertexSet) {
        if(origin->getTag() == Vertex::Tag::CENTRAL) {
            gv->setVertexColor(origin->getId(), "red");
            gv->setVertexLabel(origin->getId(), "Meat Wagons Central");
            gv->setVertexSize(origin->getId(), 40);
        }

        else if(origin->getTag() == Vertex::Tag::INTEREST_POINT) {
            gv->setVertexColor(origin->getId(), "yellow");
            gv->setVertexLabel(origin->getId(), "Point of interest");
            gv->setVertexSize(origin->getId(), 20);
        }

        else if(origin->getTag() == Vertex::Tag::PICKUP) {
            gv->setVertexColor(origin->getId(), "green");
            gv->setVertexSize(origin->getId(), 30);
        }

        else if(origin->getTag() == Vertex::Tag::DROPOFF) {
            gv->setVertexColor(origin->getId(), "magenta");
            gv->setVertexLabel(origin->getId(), "DROPOFF");
            gv->setVertexSize(origin->getId(), 30);
        }

        else {
            gv->setVertexSize(origin->getId(), 5);
        }

        for(Edge e : origin->getAdj()) {
            gv->addEdge(e.getId(), origin->getId(), e.getDest()->getId(), EdgeType::UNDIRECTED);
        }
    }
    gv->rearrange();
}

void GraphVisualizer::setPath(const vector<int> &edges, const string &edgeColor, const bool isShortestPath) {
    for(int id : edges) {
        this->gv->setEdgeColor(id, edgeColor);
        if(isShortestPath)
            this->gv->setEdgeThickness(id, 8);
        else
            gv->setEdgeThickness(id, 3);
    }
}

void GraphVisualizer::setNode(const int id, const int size, const string color, const string label="") {
    this->gv->setVertexSize(id, size);
    this->gv->setVertexColor(id, color);
    this->gv->setVertexLabel(id, label);
}

void GraphVisualizer::drawShortestPathFromThread(const unordered_set<int> &processedEdges, const unordered_set<int> &processedEdgesInv, const vector<Edge> &edges, Graph *graph) {
    thread threadProcess(&GraphVisualizer::drawShortestPath, this, processedEdges, processedEdgesInv, edges, graph);
    threadProcess.detach();
}

void GraphVisualizer::drawShortestPath(const unordered_set<int> &processedEdges, const unordered_set<int> &processedEdgesInv, const vector<Edge> &edges, Graph *graph) {
    this->gv = new GraphViewer(1, 1, false);

    // get processed path
    vector<int> edgesProcessed(processedEdges.begin(), processedEdges.end());
    this->setPath(edgesProcessed, "orange", false);

    if(processedEdgesInv.size() != 0) {
        vector<int> edgesProcessedInv(processedEdgesInv.begin(), processedEdgesInv.end());
        this->setPath(edgesProcessedInv, "magenta", false);
    }

    vector<int> edgesIds = Edge::getIds(edges);
    // draw shortest path
    this->setPath(edgesIds, "blue", true);
    this->draw(graph);
}

#endif //MEAT_WAGONS_GRAPHVISUALIZER_H