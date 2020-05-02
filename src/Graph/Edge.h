#pragma once
#ifndef EDGE_H_
#define EDGE_H_

class Vertex;

class Edge {
    private:
        int id;
        Vertex *origin;           // origin vertex
        Vertex *dest;             // destination vertex
        double weight;            // edge weight
    
    public:
        Edge() {
            this->id = -1;
            this->origin = nullptr;
            this->dest = nullptr;
            this->weight = 0;
        }

        Edge(const int &id, Vertex *dest, Vertex *origin, const double &weight) {
            this->id = id;
            this->dest = dest;
            this->origin = origin;
            this->weight = weight;
        }

        /* get methods */
        const int getId() const;
        const Vertex* getOrigin() const;
        const Vertex* getDest() const;
        const double getWeight() const;

        friend class Graph;
        friend class Vertex;
};

const int Edge::getId() const {
	return this->id;
}

const Vertex* Edge::getOrigin() const {
    return this->origin;
}

const Vertex* Edge::getDest() const {
	return this->dest;
}

const double Edge::getWeight() const {
	return this->weight;
}

#endif