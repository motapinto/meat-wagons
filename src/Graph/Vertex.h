#pragma once
#ifndef VERTEX_H_
#define VERTEX_H_

#include "../Position/Position.h"
#include "Edge.h"
#include <limits>
using namespace std;

class Vertex 
{
    public:
        enum Tag {CENTRAL, INTEREST_POINT, DEFAULT};

    private:
        int id;                         // identifier of the vertex
        Position pos;			              // content of the vertex
        vector<Edge> adj;		            // outgoing edges
        vector<Edge> invAdj;            // ingoing edges
        Tag tag = DEFAULT;              // vertex Tag

        double dist = infinite;
        double invDist = infinite;
        Vertex *path = nullptr;
        Vertex *invPath = nullptr;
        Edge edgePath;
        Edge invEdgePath;
        
        int queueIndex = 0; 		        // required by MutablePriorityQueue
        double heuristicValue = 0;      // oriented search optimization (a*)
        double invHeuristicValue = 0;
        int invQueueIndex = 0;

        bool visited = false;		        // auxiliary field
        bool invertedVisited = false;   // auxiliary field
        bool processing = false;	      // auxiliary field

        void addEdge(const int &id, Vertex *dest, const double &weight);
        const static size_t infinite = UINT_MAX;

    public:
        Vertex(const int &id, const int &x, const int &y) {
            this->id = id;
            this->pos = Position(x, y);
        }
        
        /* get methods */
        int getId() const;
        Position getPosition() const;
        vector<Edge> getAdj() const;
        double getDist() const;
        Vertex *getPath() const;
        Edge getEdgePath() const;
        bool getVisited() const;
        Tag getTag() const;
        void setTag(Vertex::Tag tag);

        bool operator<(Vertex &vertex) const; //required by MutablePriorityQueue
        friend ostream& operator<<(ostream &out, const Vertex &vertex);
        friend class Graph;
        friend class MutablePriorityQueue<Vertex>;
};

/**
 * Auxiliary function to add an outgoing edge to a vertex (this),
 * with a given destination vertex (dest) and edge weight (weight).
 */
void Vertex::addEdge(const int &id, Vertex *dest, const double &weight) {
    Edge edge = Edge(id, dest, this, weight);
	adj.push_back(edge);
	dest->invAdj.push_back(edge);
}

bool Vertex::operator<(Vertex &vertex) const {
	return this->dist < vertex.dist;
}
ostream& operator<<(ostream &out, const Vertex &vertex) {
    // shortestPath dijkstra 1 34
    out << "ID: " << vertex.id << " | Pos: ("
        << vertex.getPosition().getX() << ", "
        << vertex.getPosition().getY() << ") | Edges: ";

    for(size_t i=0; i<vertex.adj.size(); ++i) {
        out << vertex.adj.at(i).getId() << " ";
    }

    out << endl;
    return out;
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

Edge Vertex::getEdgePath() const {
    return this->edgePath;
}

bool Vertex::getVisited() const {
    return this->visited;
}

Vertex::Tag Vertex::getTag() const {
    return this->tag;
}

void Vertex::setTag(Vertex::Tag tag) {
    this->tag = tag;
}

#endif