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
            this->gv = new GraphViewer(10000, 10000, false);
        }

        ~GraphVisualizer() {
            delete gv;
        }

        void draw(Graph *graph);
        void setPath(vector<int> vert, vector<int> edges);
};

// no final tentar fazer animacao como na tp com sleeps
void GraphVisualizer::draw(Graph *graph) {
    gv->createWindow(width, height);
    //vertexes settings
    gv->defineVertexColor("black");
    gv->defineVertexSize(1);
    //edges settings
    gv->defineEdgeColor("black");
    gv->defineEdgeCurved(false);

    vector<Vertex*> vertexSet = graph->getVertexSet();

    for(Vertex *origin : vertexSet)
        gv->addNode(origin->getId(), origin->getPosition().getX() - graph->getOffsetX(), origin->getPosition().getY() - graph->getOffsetY());

    for(Vertex *origin : vertexSet) {
        if(origin->getTag() == Vertex::Tag::CENTRAL) {
            gv->setVertexColor(origin->getId(), "red");
            gv->setVertexLabel(origin->getId(), "Meat Wagons Central");
        }

        else if(origin->getTag() == Vertex::Tag::INTEREST_POINT) {
            gv->setVertexColor(origin->getId(), "yellow");
            gv->setVertexLabel(origin->getId(), "Point of interest");
        }

        for(Edge e : origin->getAdj()) {
            gv->addEdge(e.getId(), origin->getId(), e.getDest()->getId(), EdgeType::DIRECTED);
        }
    }
    gv->rearrange();
}

void GraphVisualizer::setPath(vector<int> vert, vector<int> edges) {
    for(int id : vert) {
        this->gv->setVertexColor(id, "green");
        this->gv->setVertexSize(id, 2);
    }

    for(int id : edges) {
        this->gv->setEdgeColor(id, "blue");
        this->gv->setEdgeThickness(id, 10);
    }
}

#endif //MEAT_WAGONS_GRAPHVISUALIZER_H