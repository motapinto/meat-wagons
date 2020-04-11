#pragma once
#ifndef VERTEX_H_
#define VERTEX_H_

#include <vector>
#include "Position.h"
#include "Edge.h"

using namespace std;

class Vertex {
        static int serial;          // number of Vertex objects shared with all vertices
        int id;                     // identifier of the vertex
        Position pos;			    // content of the vertex
        vector<Edge> adj;		    // outgoing edges
        
        double dist = 0;
        Vertex *path = NULL;
        int queueIndex = 0; 		// required by MutablePriorityQueue
        double heuristicValue = 0;  // oriented search optimization (a*)

        bool visited = false;		// auxiliary field
        bool processing = false;	// auxiliary field

        void addEdge(int id, Vertex *dest, double weight);

    public:
        Vertex(int x, int y) {
            this->id = serial;
            this->pos = Position(x, y);

            this->serial += 1;
        }

        Vertex(int id, int x, int y) {
            this->id = id;
            this->pos = Position(x, y);

            this->serial += 1;
        }
        
        /* get methods */
        int getId() const;
        Position getPosition() const;
        vector<Edge> Vertex::getAdj() const;
        double getDist() const;
        Vertex *getPath() const;
        bool getVisited() const;

        bool operator<(Vertex & vertex) const; //required by MutablePriorityQueue
        friend class Graph;
        friend class MutablePriorityQueue<Vertex>;
};

/**
 * Auxiliary function to add an outgoing edge to a vertex (this),
 * with a given destination vertex (dest) and edge weight (weight).
 */
void Vertex::addEdge(int id, Vertex *dest, double weight) {
	adj.push_back(Edge(id, this, dest, weight));
}

bool Vertex::operator<(Vertex & vertex) const {
	return this->dist < vertex.dist;
}

int Vertex::getId() const {
	return this->id;
}

Position Vertex::getPosition() const {
	return this->pos;
}

vector<Edge> Vertex::getAdj() const {
	return this->adj;
}

double Vertex::getDist() const {
	return this->dist;
}

Vertex* Vertex::getPath() const {
	return this->path;
}

bool Vertex::getVisited() const {
    return this->visited;
}

#endif