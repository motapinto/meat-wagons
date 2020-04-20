#pragma once
#ifndef EDGE_H_
#define EDGE_H_

class Vertex;

class Edge {
    private:
        //static int serial;        // number of Edge objects shared with all edges
        int id;
        Vertex *dest;             // origin and destination vertex
        double weight;            // edge weight
    
    public:
        Edge() {
            this->id = -1;
            this->dest = nullptr;
            this->weight = 0;
        }

        /*Edge(Vertex *dest, const double &weight) {
            this->id = Edge::serial + 1;
            this->dest = dest;
            this->weight = weight;

            this->serial += 1;
        }*/

        Edge(const int &id, Vertex *dest, const double &weight) {
            this->id = id;
            this->dest = dest;
            this->weight = weight;

            //this->serial += 1;
        }

        /* get methods */
        int getId() const;
        Vertex* getDest() const;
        double getWeight() const;

        friend class Graph;
        friend class Vertex;
};

int Edge::getId() const {
	return this->id;
}

Vertex* Edge::getDest() const {
	return this->dest;
}

double Edge::getWeight() const {
	return this->weight;
}

#endif