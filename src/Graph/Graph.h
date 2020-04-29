#pragma once
#ifndef GRAPH_H_
#define GRAPH_H_

#include <unordered_map>
#include <queue>
#include <list>
#include <limits>
#include <algorithm>
#include <cmath>
#include "MutablePriorityQueue.h"
#include "Vertex.h"

using namespace std;

class Graph {
    private:
        int offsetX;    // for Graph Viewer
        int offsetY;    // for Graph Viewer

        vector<Vertex*> vertexSet;   
        unordered_map<int, Vertex*> vertexIndexes;    //search for id and return vertex (much faster)

        vector<vector<double>> minDistance;       // used for floyd Warshall algorithm
        vector<vector<Vertex*>> next;             // used for floyd Warshall algorithm
        void dfsVisit(Vertex *origin) const;      // pre processing

        const static int infinite = 99999999;

    public:
        Vertex* findVertex(const int &id) const;
        bool addVertex(const int &id, const int &x, const int &y);
        void addPointOfInterest(Vertex* vertex);
        bool addEdge(const int &id, const int &origin, const int &dest);

        void setOffsetX(int x);
        void setOffsetY(int y);
        int getOffsetX();
        int getOffsetY();
        
        int getNumVertex() const;
        vector<Vertex*> getVertexSet() const;

        // pre processing
        bool preProcess(int origin);

        // dijkstra
        Vertex* dijkstraInit(const int origin);
        Vertex* dijkstraBackwardsInit(const int dest);
        bool dijkstraSingleSource(const int origin);
        bool dijkstra(const int origin, const int dest);
        bool getPathTo(const int dest, vector<int> &vert, vector<int> &edges) const;

        // dijkstra related
        double heuristicDistance(Vertex *origin, Vertex *dest);
        bool dijkstraOrientedSearch(const int origin, const int dest) ;
        bool dijkstraBidirectional(const int origin, const int dest);
        
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

bool Graph::preProcess(int origin) {
    auto orig = findVertex(origin);
    if (orig == nullptr) return false;

    for(auto vertex : vertexSet)
        vertex->visited = false;
    
    dfsVisit(orig);

    set<int> removed;
    // deletes nodes
    for(auto it = vertexSet.begin(); it != vertexSet.end(); it++) {
        if(!(*it)->visited) {
            vertexIndexes.erase((*it)->getId());
            removed.insert((*it)->getId());
            it = vertexSet.erase(it) - 1;
        }
    }

    // deletes outgoing edges of the deleted nodes
    for(auto vertex : vertexSet) {
        for(auto it = vertex->adj.begin(); it != vertex->adj.end(); it++) {
            if(removed.find(it->getDest()->getId()) != removed.end()) {
                it = vertex->adj.erase(it) - 1;
            }
        }
    }

    return true;
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
	vertexSet.push_back(vertex);
    vertexIndexes.insert(pair<int, Vertex*>(id, vertex));
	return true;
}

bool Graph::addEdge(const int &id, const int &origin, const int &dest) {
	auto v1 = findVertex(origin);
	auto v2 = findVertex(dest);
	if (v1 == nullptr || v2 == nullptr)
		return false;
	
    v1->addEdge(id, v2, v1->pos.euclideanDistance(v2->pos));
    v2->addEdge(id + 100000, v1, v2->pos.euclideanDistance(v1->pos));

	return true;
}

void Graph::setOffsetX(int x) {
    this->offsetX = x;
}

void Graph::setOffsetY(int y) {
    this->offsetY = y;
}

int Graph::getOffsetX() {
    return offsetX;
}

int Graph::getOffsetY() {
    return offsetY;
}

int Graph::getNumVertex() const {
	return vertexSet.size();
}

vector<Vertex*> Graph::getVertexSet() const {
	return vertexSet;
}

/**************** Dijkstra ************/
Vertex* Graph::dijkstraInit(const int origin) {
     for(auto vertex : vertexSet) {

         if(vertex->dist == 0) continue;
         vertex->visited = false;
         vertex->invertedVisited = false;
         vertex->dist = infinite;
         vertex->path = NULL;
         vertex->edgePath = Edge();
         vertex->invDist = infinite;
         vertex->invPath = NULL;
         vertex->invEdgePath = Edge();
         vertex->heuristicValue = infinite;
         vertex->invHeuristicValue = infinite;

	}

	auto start = findVertex(origin);
	start->dist = 0;
    start->heuristicValue = 0;

    return start;
}

Vertex* Graph::dijkstraBackwardsInit(const int dest){

    auto start = findVertex(dest);
    start->invDist = 0;
    start->invHeuristicValue = 0;

    return start;
}

bool Graph::dijkstraSingleSource(const int origin)  {
    auto start = dijkstraInit(origin);
    if(start == nullptr) return false;

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

                // if elem is not in queue (old dist(w) was infinite)
                if(elem->queueIndex == 0) minQueue.insert(elem);
                else minQueue.decreaseKey(elem);
            }
        }
    }
    return true;
}

bool Graph::dijkstra(const int origin, const int dest)  {
    auto start = dijkstraInit(origin);
    auto final =  findVertex(dest);

    if(start == nullptr || final == nullptr)
        return false;

	MutablePriorityQueue<Vertex> minQueue;
    minQueue.insert(start);

    while(!minQueue.empty()) {
        auto min = minQueue.extractMin();

        if(min == final) return true;

        for(auto edge : min->adj) {
            auto elem = edge.dest;

            if(elem->dist > min->dist + edge.weight) {
                elem->dist = min->dist + edge.weight;
                elem->path = min;
                elem->edgePath = edge;

                // if elem is not in queue (old dist(w) was infinite)
                if(elem->queueIndex == 0) minQueue.insert(elem);
                else minQueue.decreaseKey(elem);
            }
        }
    }
    return true;
}

bool Graph::getPathTo(const int dest, vector<int> &vert, vector<int> &edges) const {
    Vertex *final = findVertex(dest);

    if(final == nullptr || (final->path == nullptr && final->invPath == nullptr))
        return false;

    vert.push_back(final->getId());
    edges.push_back(final->getEdgePath().getId());

    while(final->path != nullptr || final->invPath != nullptr) {
        final = final->path;

        vert.push_back(final->getId());

        edges.push_back(final->getEdgePath().getId());

	  }

    reverse(vert.begin(), vert.end());
    reverse(edges.begin(), edges.end());
  
	  return true;
}

/**************** Optimizing Dijkstra ************/

double Graph::heuristicDistance(Vertex *origin, Vertex *dest) {
    return origin->getPosition().euclideanDistance(dest->getPosition());
}

// Uses a heuristic to optimize dijkstra(A*)
bool Graph::dijkstraOrientedSearch(const int origin, const int dest) {
   auto start = dijkstraInit(origin);
   auto final = findVertex(dest);

    if(start == nullptr || final == nullptr)
        return false;

   start->dist = start->getPosition().euclideanDistance(final->getPosition());

	MutablePriorityQueue<Vertex> minQueue;
    minQueue.insert(start);

    while(!minQueue.empty()) {
        auto min = minQueue.extractMin();
        min->visited = true;

        if(min == final) return false;

        for(auto edge : min->adj) {
            auto elem = edge.dest;
            auto weight = edge.weight;

            if(elem->visited) continue;


            if(min->dist + weight + heuristicDistance(elem, min) < elem->heuristicValue) {
                
                elem->heuristicValue =  min->heuristicValue + weight + heuristicDistance(elem, min);
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
    return true;
}

// Upgrades the optimization using a* with bidirectional search
bool Graph::dijkstraBidirectional(const int origin, const int dest) {

    auto start = dijkstraInit(origin);
    auto final = dijkstraBackwardsInit(dest);

    if(start == nullptr || final == nullptr) return false;

    MutablePriorityQueue<Vertex> forwardMinQueue;
    forwardMinQueue.insert(start);
    MutablePriorityQueue<Vertex> backwardMinQueue;
    backwardMinQueue.insert(final);
    vector<int> processed;
    vector<int> backward_processed;

    Vertex * forwardMin = nullptr;
    Vertex * backwardMin = nullptr;
    Vertex * middle_vertex = nullptr;

    // strict alternation between forward and backward search
    while(!forwardMinQueue.empty() && !backwardMinQueue.empty()) {
        //forward search
        forwardMin = forwardMinQueue.extractMin();
        forwardMin->visited = true;

        // check if there is an intersection
        if(find(processed.begin(), processed.end(), forwardMin->id) != processed.end()) break;

        processed.push_back(forwardMin->id);

        for(auto edge : forwardMin->adj) {
            auto elem = edge.dest;
            auto weight = edge.weight;

            if(elem->visited) continue;

            if(forwardMin->dist + weight  < elem->dist) {
                
                elem->heuristicValue =  forwardMin->heuristicValue + weight + heuristicDistance(elem, forwardMin);
                elem->dist = forwardMin->dist + weight;
                elem->path = forwardMin;
                elem->edgePath = edge;
                
                // if elem is not in queue  [old dist(w) was infinite]
                if(elem->queueIndex == 0) 
                  forwardMinQueue.insert(elem);
                else 
                  forwardMinQueue.decreaseKey(elem);
            }
        }

        if(find(backward_processed.begin(), backward_processed.end(), forwardMin->id) != backward_processed.end()) {
            middle_vertex = forwardMin;
            break;
        }

        //backward search
        backwardMin = backwardMinQueue.extractMin();
        backwardMin->visited = true;

        backward_processed.push_back(backwardMin->id);

        for(auto edge : backwardMin->invAdj) {
            auto elem = edge.origin;
            auto weight = edge.weight;

            if(elem->visited) continue;

            if(backwardMin->invDist + weight < elem->invDist) {
                
                elem->invHeuristicValue =  backwardMin->invHeuristicValue + weight + heuristicDistance(elem, backwardMin);
                elem->invDist = backwardMin->invDist + weight;
                elem->invPath = backwardMin;
                elem->invEdgePath = edge;

                
                // if elem is not in queue
                if(elem->invQueueIndex == 0) { //old dist(w) was infinite
                    backwardMinQueue.insert(elem);
                    elem->invQueueIndex = elem->queueIndex;
                }

                else {
                    backwardMinQueue.decreaseKey(elem);
                    elem->invQueueIndex = elem->queueIndex;
                }
            }
        }

        if(find(processed.begin(), processed.end(), backwardMin->id) != processed.end()){
            middle_vertex = backwardMin;
            break;
        }

    }

    int min_dist = middle_vertex->getDist() + middle_vertex->invDist;

    while(!forwardMinQueue.empty()) {
        forwardMin = forwardMinQueue.extractMin();

        if(forwardMin->getDist() + forwardMin->invDist < min_dist) {
            min_dist = forwardMin->getDist() + forwardMin->invDist < min_dist;
            middle_vertex = forwardMin;
        }
    }

    while(!backwardMinQueue.empty()) {
        backwardMin = backwardMinQueue.extractMin();

        if(backwardMin->getDist() + backwardMin->invDist < min_dist){
            min_dist = backwardMin->getDist() + backwardMin->invDist < min_dist;
            middle_vertex = backwardMin;
        }
    }

    while(middle_vertex->invPath != nullptr) {
        middle_vertex->invPath->path = middle_vertex;
        middle_vertex->invPath->edgePath = middle_vertex->invEdgePath;
        middle_vertex = middle_vertex->invPath;
    }

    return true;
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