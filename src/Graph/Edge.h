#pragma once
#ifndef EDGE_H_
#define EDGE_H_

class Vertex;

class Edge {
    private:
        int id;
        Vertex *dest;             // origin and destination vertex
        double weight;            // edge weight
    
    public:
        Edge() {
            this->id = -1;
            this->dest = nullptr;
            this->weight = 0;
        }

        Edge(const int &id, Vertex *dest, const double &weight) {
            this->id = id;
            this->dest = dest;
            this->weight = weight;
        }

        /* get methods */
        const int getId() const;
        const Vertex* getDest() const;
        const double getWeight() const;

        friend class Graph;
        friend class Vertex;
};

const int Edge::getId() const {
	return this->id;
}

const Vertex* Edge::getDest() const {
	return this->dest;
}

const double Edge::getWeight() const {
	return this->weight;
}

#endif