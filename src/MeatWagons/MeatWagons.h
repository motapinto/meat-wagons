#pragma once
#ifndef MEAT_WAGONS_MEATWAGONS_H
#define MEAT_WAGONS_MEATWAGONS_H

#include <unordered_set>
#include <set>
#include <time.h>
#include <algorithm>
#include "Request.h"
#include "../Graph/Reader.h"
#include "../GraphViewer/GraphVisualizer.h"
#include "Wagon.h"

class MeatWagonsException : public std::exception {
    public:
        MeatWagonsException(string  msg) : msg_(std::move(msg)) {}
        string getMessage() const {return(msg_);}

    private:
        string msg_;
};

class MeatWagons {
    private:
        int central;
        Graph *graph = nullptr;
        GraphVisualizer *viewer = new GraphVisualizer(600, 600);
        unordered_map<int, Vertex*> pointsOfInterest;
        string graphName;
        set<Wagon> wagons;
        multiset<Request> requests;
        int zoneMaxDist;
        bool processed = false;

    public:
        MeatWagons(const int wagons, const int maxDist) {
            for(int i = 0; i < wagons; i++) this->wagons.insert(Wagon(i, 2));
            this->zoneMaxDist = maxDist;
        }

        int getCentral() const;
        void setCentral(const int &id);

        string getGraphName() const;
        void setGraphName(string name);

        int getMaxDist() const;
        void setMaxDist(const int max);

        Graph* getGraph() const;
        void setGraph(const string path);
        void showGraph();

        void preProcess(int node);
        void shortestPath(int option, int origin, int dest);

        void listWagons() const;
        void addWagon(const int capacity);
        void removeWagon(const int id, const int capacity);

        void listRequests() const;

        void deliver(const int iteration);
        int chooseDropOff(const set<Vertex*> &pickupNodes);
        vector<Request> groupRequests(const int capacity);
        Vertex* getNearestNeighbour(Vertex *node,  const set<Vertex*> &neighbours);
        int tspPath(set<Vertex*> &tspNodes, vector<int> &tspPath);
        Delivery* drawDeliveries(int wagonIndex, int deliveryIndex);

        void firstIteration();
        void secondIteration();
        void thirdIteration();
};

int MeatWagons::getCentral() const {
    return this->central;
}

void MeatWagons::setCentral(const int &id) {
    this->central = id;
}

string MeatWagons::getGraphName() const {
    return this->graphName;
}

void MeatWagons::setGraphName(string name) {
    this->graphName = name;
}

int MeatWagons::getMaxDist() const {
    return this->zoneMaxDist;
}

void MeatWagons::setMaxDist(const int max) {
    this->zoneMaxDist = max;
}

Graph* MeatWagons::getGraph() const {
    return this->graph;
}

void MeatWagons::setGraph(const string graphPath) {
    Reader graphReader = Reader(graphPath);
    Graph* graphRead = new Graph();

    if(!graphReader.readGraph(graphRead, central, pointsOfInterest))
        throw MeatWagonsException("Graph is null");
    if(!graphReader.readRequests(requests))
        throw MeatWagonsException("Graph is null");

    this->processed = false;
    this->graph = graphRead;
    this->graphName = graphPath.substr(graphPath.find_last_of('/') + 1);
    //this->showGraph();
}

void MeatWagons::showGraph() {
    this->viewer = new GraphVisualizer(600, 600);
    this->viewer->draw(this->graph);
}

void MeatWagons::preProcess(int node) {
    if(this->graph == nullptr) throw MeatWagonsException("Graph is null");
    if(!this->graph->preProcess(node)) throw MeatWagonsException("Vertex does not exist");

    for(auto it = this->requests.begin(); it != this->requests.end(); it++) {
        if(this->graph->findVertex((*it).getDest()) == nullptr) {
            it = this->requests.erase(it);
            it--;
        }
    }

    this->processed = true;
    //this->showGraph();
}

void MeatWagons::shortestPath(int option, int origin, int dest) {
    if(this->graph == nullptr) throw MeatWagonsException("Graph is null");

    unordered_set<int> processedEdges, processedEdgesInv;

    switch (option) {
        case 1: if (!this->graph->dijkstra(origin, dest, processedEdges)) throw MeatWagonsException("Vertex not found"); break;
        case 2: if (!this->graph->dijkstraOrientedSearch(origin, dest, processedEdges)) throw MeatWagonsException("Vertex was not found");break;
        case 3: if (!this->graph->dijkstraBidirectional(origin, dest, processedEdges, processedEdgesInv)) throw MeatWagonsException("Vertex was not found");break;
    }

    this->viewer = new GraphVisualizer(600, 600);

    // get processed path
    vector<int> edgesProcessed(processedEdges.begin(), processedEdges.end());
    this->viewer->setPath(edgesProcessed, "orange", false);

    if(processedEdgesInv.size() != 0) {
        vector<int> edgesProcessedInv(processedEdgesInv.begin(), processedEdgesInv.end());
        this->viewer->setPath(edgesProcessedInv, "magenta", false);
    }

    // get shortest path
    vector<int> edges;
    this->graph->getPathTo(dest, edges);

    // draw shortest path
    this->viewer->setPath(edges, "blue", true);
    this->viewer->draw(this->graph);
}

void MeatWagons::listWagons() const {
    for (auto it = wagons.begin(); it != wagons.end(); it++) {
        cout << "[Wagon " << it->getId() << "] Capacity " << it->getCapacity() << " available at "
             << it->getNextAvailableTime() << endl;
    }
    cout << endl;
}

void MeatWagons::addWagon(const int capacity) {
    wagons.insert(Wagon(wagons.size(), capacity));
}

void MeatWagons::removeWagon(const int id, const int capacity) {
    wagons.erase(Wagon(id, capacity));
}

void MeatWagons::listRequests() const {
    for (auto it = requests.begin(); it != requests.end(); it++) {
        cout << "Prisoner " << (*it).getPrisoner() << " to be received in node " << (*it).getDest() << " with priority "
             << (*it).getPriority() << " at " << (*it).getArrival() << " and deliver at " << (*it).getDelivery() << endl << endl;
    }
}

void MeatWagons::deliver(int iteration) {
    if(this->graph == nullptr) throw MeatWagonsException("Graph is null");
    if(!this->processed) throw MeatWagonsException("Graph was not pre processed");
    if(this->requests.size() == 0) return;

    //if(!this->graph->dijkstraSingleSource(this->central)) throw MeatWagonsException("Vertex was not found"); not working dont know ehyyyy

    switch (iteration) {
        case 1: this->firstIteration(); break;
        case 2: this->secondIteration(); break;
        //case 3: this->thirdIteration(); break;
        default:
            break;
    }
}

int MeatWagons::chooseDropOff(const set<Vertex*> &pickupNodes) {
    srand((unsigned) time(0));
    int random_vertex;
    int id;

    while(true){
        random_vertex = (rand() % graph->getNumVertex());
        id = graph->getVertexSet()[random_vertex]->getId();

        auto it = find_if(begin(pickupNodes), end(pickupNodes), [=](const Vertex* v) {return v->getId() == id;});
        if(it != pickupNodes.end())
            continue;

        return id;
    }
}

vector<Request> MeatWagons::groupRequests(const int capacity){
    vector <Request> group;
    int max_dist = 0, dist;

    auto it = requests.begin();
    auto max_dist_request = it;
    Vertex* initial_vert = this->graph->findVertex((*it).getDest());
    group.push_back((*it));

    /*it++;
    int i = 0;
    for(; it != requests.end(); it++) {
        Vertex *vert = this->graph->findVertex((*it).getDest());
        dist = vert->getPosition().euclideanDistance(initial_vert->getPosition());

        if(dist < this->zoneMaxDist) {
            if(group.size() < capacity) {
                group.push_back(*it);

                if(dist > max_dist) {
                    max_dist = dist;
                    max_dist_request = it;
                }
            }
            else if(dist < max_dist) {
                group.erase(max_dist_request);
                max_dist = dist;

                // Check if there is a dist bigger then max_dist in the array
                for(auto itr = group.begin(); itr != group.end(); itr++) {
                    auto *vertex = this->graph->findVertex((*itr).getDest());
                    dist = vertex->getPosition().euclideanDistance(initial_vert->getPosition());

                    if(dist > max_dist) {
                        max_dist = dist;
                        max_dist_request = itr;
                    }
                }

                group.push_back(*it);
            }
        }
    }*/

    return group;
}

Vertex* MeatWagons::getNearestNeighbour(Vertex *node,  const set<Vertex*> &neighbours) {
    double nearestDistance = this->graph->findVertex(this->central)->getPosition().euclideanDistance((*neighbours.begin())->getPosition());
    auto nearestNeighbour = *neighbours.begin();

    for(auto it = ++neighbours.begin(); it != neighbours.end(); it++) {
        auto currDistance = this->graph->findVertex(this->central)->getPosition().euclideanDistance((*it)->getPosition()    );
        if(currDistance < nearestDistance) {
            nearestDistance = currDistance;
            nearestNeighbour = *it;
        }
    }

    return nearestNeighbour;
    return nullptr;
}

int MeatWagons::tspPath(set<Vertex*> &tspNodes, vector<int> &tspPath) {
    unordered_set <int> processedEdges, processedInvEdges;
    int weight = 0;
    double totalTime = 0;

    // Because tspNodes is a set ordered by dist -> the first element is the closest to the central
    Vertex *closest = *tspNodes.begin();
    this->graph->dijkstraBidirectional(central, closest->getId(), processedEdges, processedInvEdges);
    weight += this->graph->getPathTo(closest->getId(), tspPath);
    tspNodes.erase(tspNodes.begin());

    while(!tspNodes.empty()) {
        Vertex *next = getNearestNeighbour(closest, tspNodes);
        this->graph->dijkstraBidirectional(closest->getId(), next->getId(), processedEdges, processedInvEdges);
        weight += this->graph->getPathTo(next->getId(), tspPath);
        tspNodes.erase(next);
        closest = next;
    }

    return weight;
}

Delivery* MeatWagons::drawDeliveries(int wagonIndex, int deliveryIndex) {
    if(wagonIndex > this->wagons.size()) return nullptr;

    this->viewer = new GraphVisualizer(600, 600);
    Delivery * delivery = next(this->wagons.begin(), wagonIndex)->getDeliveries().at(deliveryIndex);

    for(auto request : delivery->getRequests()) {
        this->graph->findVertex(request.getDest())->setTag(Vertex::PICKUP);
        this->graph->findVertex(delivery->getDropOff())->setTag(Vertex::DROPOFF);
    }

    this->viewer->setPath(delivery->getForwardPath(), "blue", true);
    this->viewer->draw(this->graph);

    return delivery;
}

/**
 * Iteration: Using a single van with capacity equal to 1 (receive prisoner -> deliver to dropOff location -> return to central)
 */
void MeatWagons::firstIteration() {
    if(wagons.size() != 1)  throw MeatWagonsException("Wrong iteration configuration");
    if(wagons.begin()->getCapacity() != 1)  throw MeatWagonsException("Wrong iteration configuration");

    unordered_set<int> processedEdges, processedInvEdges;
    for(Wagon wagon : this->wagons) wagon.init();

    while(!this->requests.empty()) {
        // requests are ordered by pickup time
        Request request = *this->requests.begin();
        this->requests.erase(this->requests.begin());

        // get wagon
        auto wagonIt = this->wagons.end();
        wagonIt --;
        auto wagon = *wagonIt;
        this->wagons.erase(wagonIt);

        // central -> dropOff path
        vector<int> edgesForwardTrip;
        set<Vertex*> pickupNodes;

        // pickup prisoner path
        this->graph->dijkstraBidirectional(central, request.getDest(), processedEdges, processedInvEdges);
        int weight = this->graph->getPathTo(request.getDest(), edgesForwardTrip);
        pickupNodes.insert(this->graph->findVertex(request.getDest()));

        // deliver prisoner path
        int dropOffNode = chooseDropOff(pickupNodes);
        this->graph->dijkstraBidirectional(request.getDest(), dropOffNode, processedEdges, processedInvEdges);
        weight += graph->getPathTo(dropOffNode, edgesForwardTrip);

        // return to central path (bidirectional graph -> path is equal to edgesForwardTrip)
        weight *= 2;

        // add delivery to wagon
        Time lastDeliveryTime = wagon.getDeliveries().size() > 0 ? wagon.getDeliveries().at(wagon.getDeliveries().size() - 1)->getEnd() : request.getArrival();
        Delivery *delivery = new Delivery(lastDeliveryTime, {request}, edgesForwardTrip, weight, dropOffNode);
        wagon.addDelivery(delivery);

        // wagon now is back at the central
        this->wagons.insert(wagon);
    }
}

/**
 * Iteration: Using a single van with capacity > 1 (receive prisoner -> deliver to dropOff location -> return to central)
 */
void MeatWagons::secondIteration() {
    if(wagons.size() != 1)  throw MeatWagonsException("Wrong iteration configuration");
    if(wagons.begin()->getCapacity() <= 1)  throw MeatWagonsException("Wrong iteration configuration");

    unordered_set<int> processedEdges;
    for(Wagon wagon : this->wagons) wagon.init();

    while(!requests.empty()) {
        // get wagon with max capacity
        auto wagonIt = --this->wagons.end();
        auto wagon = *wagonIt;
        this->wagons.erase(wagonIt);

        // returned grouped requests and insert tsp nodes
        vector<Request> groupedRequests = groupRequests(wagon.getCapacity());
        set<Vertex*> tspNodes;
        for(auto r : groupedRequests) {
            this->requests.erase(r);
            auto tspNode = this->graph->findVertex(r.getDest());
            tspNodes.insert(tspNode);
        }

        // chose drop off node and calculates tsp path
        int dropOffNode = chooseDropOff(tspNodes);
        tspNodes.insert( this->graph->findVertex(dropOffNode));
        vector<int> tspPath;
        int weight = this->tspPath(tspNodes, tspPath);

        // gets the time of the latest request (groupedRequests is a set ordered by arrival time)
        Time deliveryTime = (*(--groupedRequests.end())).getArrival();

        // add delivery to wagon
        Delivery *delivery = new Delivery(deliveryTime, groupedRequests, tspPath, weight, dropOffNode);
        wagon.addDelivery(delivery);

        // wagon now is back at the central
        this->wagons.insert(wagon);
    }

}

/**
 * Iteration: Using many vans with capacity > 1 (receive prisoner -> deliver to dropOff location -> return to central)
 */
void MeatWagons::thirdIteration() {
}

#endif //MEAT_WAGONS_MEATWAGONS_H