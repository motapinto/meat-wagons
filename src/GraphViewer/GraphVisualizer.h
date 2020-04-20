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
        GraphVisualizer(int width, int height) {
            this->width = width;
            this->height = height;
            this->gv = new GraphViewer(width, height, false);
        }

        ~GraphVisualizer() {
            delete gv;
        }

        void draw(Graph *graph);
};

void GraphVisualizer::draw(Graph *graph) {
    gv->createWindow(width, height);

    //vertexes settings
    gv->defineVertexColor("blue");
    gv->defineVertexSize(1);
    //edges settings
    gv->defineEdgeColor("black");
    //gv->setEdgeCurved(false);

    for(Vertex *origin : graph->getVertexSet()) {
        gv->addNode(origin->getId(), origin->getPosition().getX(), origin->getPosition().getY());

        if(origin->getTag() == Vertex::Tag::CENTRAL)
            gv->setVertexColor(origin->getId(), "red");
        else if(origin->getTag() == Vertex::Tag::INTEREST_POINT)
            gv->setVertexColor(origin->getId(), "yellow");

        for(Edge e : origin->getAdj())
            gv->addEdge(e.getId(), origin->getId(), e.getDest()->getId(), EdgeType::DIRECTED);
    }
}

#endif //MEAT_WAGONS_GRAPHVISUALIZER_H