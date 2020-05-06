#pragma once
#ifndef MEAT_WAGONS_GRAPHVISUALIZER_H
#define MEAT_WAGONS_GRAPHVISUALIZER_H

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
    void draw(Graph *graph);
    void setPath(const vector<int> &edges, const string &edgeColor, const bool isShortestPath = false);
    void reset();
};

GraphViewer* GraphVisualizer::getViewer() const {
    return gv;
}

// no final tentar fazer animacao como na tp com sleeps
void GraphVisualizer::draw(Graph *graph) {
    gv->createWindow(width, height);
    //vertexes settings
    gv->defineVertexColor("black");
    //edges settings
    gv->defineEdgeColor("gray");
    gv->defineEdgeCurved(false);

    vector<Vertex*> vertexSet = graph->getVertexSet();

    for(Vertex *origin : vertexSet)
        gv->addNode(origin->getId(), origin->getPosition().getX() - graph->getOffsetX(), origin->getPosition().getY() - graph->getOffsetY());

    int i = 0;
    for(Vertex *origin : vertexSet) {
        i++;
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

#endif //MEAT_WAGONS_GRAPHVISUALIZER_H