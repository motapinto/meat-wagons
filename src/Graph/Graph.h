#pragma once
#ifndef GRAPH_H_
#define GRAPH_H_

#include <vector>
#include <unordered_map>
#include <queue>
#include <list>
#include <limits>
#include <cmath>
#include "MutablePriorityQueue.h"
#include "Vertex.h"
#include<bits/stdc++.h> 

using namespace std;

class Graph {
    private:
        vector<Vertex*> vertexSet;   
        unordered_map<int, Vertex*> vertexIndexes; //search for id and return vertex (much faster)

        vector<vector<double>> minDistance;       // used for floyd Warshall algorithm
        vector<vector<Vertex*>> next;             // used for floyd Warshall algorithm
        void dfsVisit(Vertex *origin) const;      // pre processing

        const static int infinite = 99999999;

    public:
        Graph() {
        }

        Graph(vector<Vertex*> vertexSet) {
            this->vertexSet = vertexSet;
        }

        Vertex* findVertex(const int &id) const;
        bool addVertex(const int &id, const int &x, const int &y);
        bool addEdge(const int &id, const int &orig, const int &dest);
        
        int getNumVertex() const;
        vector<Vertex*> getVertexSet() const;

        // pre processing
        void removeUnvisited(Vertex *origin);

        // dijkstra
        Vertex* dijkstraInit(const int origin, bool isOriented = false);
        void dijkstraSingleSource(const int origin);
        void dijkstraSingleSource(const int origin, const int dest);
        vector<int> getPathTo(const int origin, const int dest) const;

        // dijkstra related
        void reverseGraph();
        double heuristicDistance(Vertex *origin, Vertex *dest);
        void dijkstraOrientedSearch(const int origin, const int dest) ; 
        void dijkstraBidirectional(const int origin, const int dest);
        
        // all pairs
        void floydWarshallShortestPath();
        vector<int> getfloydWarshallPath(const int origin, const int dest) const;
};

/**************** Pre processing ************/
void Graph::dfsVisit(Vertex *origin) const {
    origin->visited = true;

	for(auto edge : origin->adj) {
	    if(!edge.dest->visited) {
	        dfsVisit(edge.dest);
	    }
	}
}

void Graph::removeUnvisited(Vertex *origin) {
    for(auto origin : vertexSet)
        origin->visited = false;
    
    dfsVisit(origin);

    // Removes vertices and calls for each his Vertex deconstructor (erasing the edges also)
    for(auto it = vertexSet.begin(); it != vertexSet.end(); it++) {
        if(!(*it)->visited) {
            vertexIndexes.erase((*it)->getId());
            it = vertexSet.erase(it) - 1;
        }
    }

}
/**************** Usual operations ************/
Vertex* Graph::findVertex(const int &id) const {
    auto it = vertexIndexes.find(id);
	return it == vertexIndexes.end() ? nullptr : it->second;
}

bool Graph::addVertex(const int &id, const int &x, const int &y) {
	if (findVertex(id) != nullptr)
		return false;
    
    auto vertex = new Vertex(id, x, y);
	vertexSet.push_back(vertex));
    vertexIndexes.insert(pair<int, Vertex*>(id, vertex));
	return true;
}

bool Graph::addEdge(const int &id, const int &sourc, const int &dest) {
	auto v1 = findVertex(sourc);
	auto v2 = findVertex(dest);
	if (v1 == nullptr || v2 == nullptr)
		return false;
	
    v1->addEdge(id, v2, v1->pos.euclidianDistance(v2->pos));
    v2->addEdge(id + 1, v1, v2->pos.euclidianDistance(v2->pos));
	return true;
}

int Graph::getNumVertex() const {
	return vertexSet.size();
}

vector<Vertex*> Graph::getVertexSet() const {
	return vertexSet;
}

/**************** Dijkstra ************/

Vertex* Graph::dijkstraInit(const int origin, bool isOriented = true) {
     for(auto vertex : vertexSet) {
        vertex->visited = false;
        vertex->invertedVisited = false;
	    vertex->dist = infinite;
	    vertex->path = NULL;
        vertex->edgePath = Edge();
        if(isOriented) vertex->heuristicValue = 0;
	}

	auto start = findVertex(origin);
	start->dist = 0;
    if(isOriented) start->heuristicValue = 0;

    return start;
}

void Graph::dijkstraSingleSource(const int origin)  {
    auto start = dijkstraInit(origin, false);

	MutablePriorityQueue<Vertex> minQueue;
    minQueue.insert(start);

    while(!minQueue.empty()) {
        auto min = minQueue.extractMin();
        min->visited = true;

        for(auto edge : min->adj) {
            auto elem = edge.dest;

            if(elem->dist > min->dist + edge.weight) {
                elem->dist = min->dist + edge.weight;
                elem->path = min;
                elem->edgePath = edge;

                // if elem is not in queue
                if(elem->queueIndex == 0) { //old dist(w) was infinite
                    minQueue.insert(elem);
                }

                else {
                    minQueue.decreaseKey(elem);
                }
            }
        }
    }

}

void Graph::dijkstraSingleSource(const int origin, const int dest)  {
    auto start = dijkstraInit(origin, false);
    auto dest =  findVertex(dest);

	MutablePriorityQueue<Vertex> minQueue;
    minQueue.insert(start);

    while(!minQueue.empty()) {
        auto min = minQueue.extractMin();
        min->visited = true;

        if(min == dest) return;

        for(auto edge : min->adj) {
            auto elem = edge.dest;

            if(elem->dist > min->dist + edge.weight) {
                elem->dist = min->dist + edge.weight;
                elem->path = min;

                // if elem is not in queue
                if(elem->queueIndex == 0) { //old dist(w) was infinite
                    minQueue.insert(elem);
                }

                else {
                    minQueue.decreaseKey(elem);
                }
            }
        }
    }

}

vector<int> Graph::getPathTo(const int sourc, const int dest) const {
	vector<int> res;
    Vertex *start = findVertex(sourc);
    Vertex *end = findVertex(dest);

    if(start == nullptr || end == nullptr || start->dist == infinite) 
        return res;

    res.insert(res.begin(), end->getId());
    while(end->path != nullptr) {
        end = end->path;
        res.insert(res.begin(), end->getId());
	}

	return res;
}


/**************** Optimizing Dijkstra ************/
void Graph::reverseGraph() {
    for(auto vert : vertexSet)
        for(auto edge : vert->adj)
            addEdge(edge.id, edge.dest, edge.orig);
}

double Graph::heuristicDistance(Vertex *origin, Vetrex *dest) {
    return origin->getPosition().euclidianDistance(dest->getPosition());
}

// Uses a heurisitc to optimize dijkstra(A*)
void Graph::dijkstraOrientedSearch(const int origin, const int dest) {
   auto start = dijkstraInit(origin);
   auto final = findVertex(dest);

   start->dist = start->getPosition.euclidianDistance(final->getPosition());

	MutablePriorityQueue<Vertex> minQueue;
    minQueue.insert(start);

    while(!minQueue.empty()) {
        auto min = minQueue.extractMin();
        min->visited = true;

        if(min == final) return;

        for(auto edge : min->adj) {
            auto elem = edge->dest;
            auto weight = edge->weight;

            if(elem->visited) continue;


            if(min->dist + weight + heurisitc(elem, dest) < elem->heuristicValue) {
                
                elem->heuristicValue =  min->heuristicValue + weight + heurisitc(elem, dest);
                elem->dist = min->heuristicValue + weight; 
                elem->path = min;
                elem->edgePath = edge;
                
                // if elem is not in queue
                if(elem->queueIndex == 0) { //old dist(w) was infinite
                    minQueue.insert(elem);
                }

                else {
                    minQueue.decreaseKey(elem);
                }
            }
        }
    }

}

// Upgrades the optimization using a* with bidirectional search
void Graph::dijkstraBidirectional(const int origin, const int dest) {
    reverseGraph();

    auto start = dijkstraInit(origin);
    auto final = dijkstraInit(dest);

    if(start == nullptr || final == nullptr) return;

    MutablePriorityQueue<Vertex> forwardMinQueue;
    forwardMinQueue.insert(start);
    MutablePriorityQueue<Vertex> backwardMinQueue;
    backwardMinQueue.insert(final);
    vector<int> processed;

    // strict alternation between forward and backward search
    while(!forwardMinQueue.empty() && !backwardMinQueue.empty()) {
        //forward search
        auto forwardMin = forwardMinQueue.extractMin();
        forwardMin->visited = true;

        // check if there is an intersection
        if(find(processed.begin(), processed.end(), forwardMin->id) != processed.end()) 
            break;
        processed.push_back(forwardMin->id);

        if(forwardMin == final) return;

        for(auto edge : forwardMin->adj) {
            auto elem = edge->dest;
            auto weight = edge->weight;

            if(elem->visited) continue;

            if(forwardMin->dist + weight + heurisitc(elem, dest) < elem->heuristicValue) {
                
                elem->heuristicValue =  forwardMin->heuristicValue + weight + heurisitc(elem, dest);
                elem->dist = forwardMin->heuristicValue + weight; 
                elem->path = forwardMin;
                elem->edgePath = edge;
                
                // if elem is not in queue
                if(elem->queueIndex == 0) { //old dist(w) was infinite
                    forwardMinQueue.insert(elem);
                }

                else {
                    forwardMinQueue.decreaseKey(elem);
                }
            }
        }

        //backward search
        auto backwardMin = backwardMinQueue.extractMin();
        backwardMin->visited = true;

        // check if there is an intersection
        if(find(processed.begin(), processed.end(), backwardMin->id) != processed.end()) 
            break;
        processed.push_back(backwardMin->id);

        if(backwardMin == final) return;

        for(auto edge : backwardMin->adj) {
            auto elem = edge->dest;
            auto weight = edge->weight;

            if(elem->visited) continue;

            if(backwardMin->dist + weight + heurisitc(elem, dest) < elem->heuristicValue) {
                
                elem->heuristicValue =  backwardMin->heuristicValue + weight + heurisitc(elem, dest);
                elem->dist = backwardMin->heuristicValue + weight; 
                elem->invPath = backwardMin;
                elem->invEdgePath = edge;
                
                // if elem is not in queue
                if(elem->queueIndex == 0) { //old dist(w) was infinite
                    backwardMinQueue.insert(elem);
                }

                else {
                    backwardMinQueue.decreaseKey(elem);
                }
            }
        }
    }
}

/**************** All Pairs Shortest Path  ***************/

void Graph::floydWarshallShortestPath() {
	int vertSize = this->vertexSet.size();

	this->minDistance.resize(vertSize);
	for(auto &elem : this->minDistance) elem.resize(vertSize);

    this->next.resize(vertSize);
    for(auto &elem : next) elem.resize(vertSize);

    // this->minDistance -> <vector<vector<double>> that stores distance between i and j
    // this->next        -> <vector<vector<Vertex<T>*>> that stores the path between i and j
    for(int i = 0; i < vertSize; i++) {
        for(int j = 0; j < vertSize; j++) {
            this->minDistance.at(i).at(j) = infinite;
            this->next.at(i).at(j) = nullptr;
        }
    }

    for(Vertex *vertex : this->vertexSet) {
        for(Edge &edge : vertex->adj) {
            int u = -1;
            int v = -1;

            for(int k = 0; k < vertSize; k++) {
                if(this->vertexSet.at(k)->id == vertex->id) u = k;
                if(this->vertexSet.at(k)->id == edge.dest->id) v = k;
            }

            // for each edge dist[u][v] = weight(u, v)
            this->minDistance.at(u).at(v) = edge.weight;
            this->next.at(u).at(v) = edge.dest;
        }
    }

    // for each vertex dist[v][v] = 0
    for(int i = 0; i < vertSize; i++) {
        this->minDistance.at(i).at(i) = 0;
        this->next.at(i).at(i) = this->vertexSet.at(i);
    }

    for(int k = 0; k < vertSize; k++) {
        for (int i = 0; i < vertSize; i++) {
            for(int j = 0; j < vertSize; j++) {
                if(this->minDistance.at(i).at(j) > this->minDistance.at(i).at(k) + this->minDistance.at(k).at(j)) {
                    this->minDistance.at(i).at(j) = this->minDistance.at(i).at(k) + this->minDistance.at(k).at(j);
                    this->next.at(i).at(j) = this->next.at(i).at(k);
                }
            }
        }
    }
}

vector<int> Graph::getfloydWarshallPath(const int orig, const int dest) const {
	vector<int> res;
    int vertSize = this->vertexSet.size();

    Vertex *init, *final;
    init = this->findVertex(orig);
    final = this->findVertex(dest);
    if(init == nullptr || final == nullptr) return res;

    int initIndex, finalIndex;
    for(int i = 0; i < vertSize; i++)
        if(this->vertexSet.at(i)->id == init->id) initIndex = i;

    for(int i = 0; i < vertSize; i++)
        if(this->vertexSet.at(i)->id == final->id) finalIndex = i;

    if(this->next.at(initIndex).at(finalIndex) == nullptr) return res;

    res.push_back(init->id);
    while(init->id != final->id) {
        init = next.at(initIndex).at(finalIndex);
        res.push_back(init->id);
        for(int i = 0; i < vertSize; i++)
            if(this->vertexSet.at(i)->id == init->id) initIndex = i;
    }

	return res;
}

#endif 