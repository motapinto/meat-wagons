#pragma once
#ifndef EDGE_H_
#define EDGE_H_

class Vertex;

class Edge {
    private:
        static int serial;        // number of Edge objects shared with all edges
        int id;
        Vertex *orig, *dest;      // origin and destination vertex
        double weight;            // edge weight
    
    public:
        Edge() {
            this->id = -1;
            this->orig = nullptr;
            this->dest = nullptr;
            this->weight = 0;
        }

        Edge(Vertex *orig, Vertex *dest, const double &weight) {
            this->id = Edge::serial + 1;
            this->orig = orig;
            this->dest = dest;
            this->weight = weight;

            this->serial += 1;
        }

        Edge(const int &id, Vertex *orig, Vertex *dest, const double &weight) {
            this->id = id;
            this->orig = orig;
            this->dest = dest;
            this->weight = weight;

            this->serial += 1;
        }

        /* get methods */
        int getId() const;
        Vertex* Edge::getOrig() const;
        Vertex* Edge::getDest() const;
        double Edge::getWeight() const;

        friend class Graph;
        friend class Vertex;
};

int Edge::getId() const {
	return this->id;
}

Vertex* Edge::getOrig() const {
	return this->orig;
}

Vertex* Edge::getDest() const {
	return this->dest;
}

double Edge::getWeight() const {
	return this->weight;
}

#endif