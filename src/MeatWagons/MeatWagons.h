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
        vector<Vertex*> pointsOfInterest;
        string graphName;
        set<Wagon> wagons;
        multiset<Request*> requests;
        int zoneMaxDist;
        bool processed = false;
        const static int averageVelocity = 9;

    public:
        MeatWagons(const int wagons) {
            this->wagons.insert(Wagon(0, 1));
            this->zoneMaxDist = 2000;
        }

        int getCentral() const;
        void setCentral(const int &id);

        string getGraphName() const;

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
        Wagon getWagon() const;

        void listRequests() const;

        void deliver(const int iteration);
        int chooseDropOff(const set<Vertex*> &pickupNodes);
        vector<Request*> groupRequests(const int capacity);
        Vertex* getNearestNeighbour(Vertex *node,  const set<Vertex*> &neighbours);
        int tspPath(set<Vertex*> &tspNodes, vector<Edge> &tspPath);
        Time setDeliveriesTime(const vector<Edge> &tspPath, vector<Request *> &groupedRequests, const Time &latestRequest);
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

    if(!graphReader.readGraph(graphRead, central))
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

    for(int i = 0; i < this->requests.size(); i++) {
        auto it = next(this->requests.begin(), i);
        Request *r = *it;
        Vertex *vert = this->graph->findVertex((r)->getDest());
        if(vert == nullptr) {
            this->requests.erase(it);
            i--;
        } else {
            this->pointsOfInterest.push_back(vert);
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
    vector<Edge> edges;
    this->graph->getPathTo(dest, edges);
    vector<int> edgesIds = Edge::getIds(edges);
    // draw shortest path
    this->viewer->setPath(edgesIds, "blue", true);
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

        cout << "Prisoner " << (*it)->getPrisoner() << " to be received in node " << (*it)->getDest() << " with priority "
             << (*it)->getPriority() << " at " << (*it)->getArrival() << endl << endl;
    }
}

void MeatWagons::deliver(int iteration) {
    if(!this->processed) this->preProcess(central);
    if(this->requests.size() == 0) return;
    if(!this->graph->dijkstraOriginal(central)) return;

    switch (iteration) {
        case 1: this->firstIteration(); break;
        case 2: this->secondIteration(); break;
        case 3: this->thirdIteration(); break;
        default: break;
    }
}

int MeatWagons::chooseDropOff(const set<Vertex*> &pickupNodes) {
    srand((unsigned) time(0));
    int id, randomId;

    while(true){
        randomId = (rand() % this->requests.size());
        id = this->pointsOfInterest.at(randomId)->getId();

        auto it = find_if(begin(pickupNodes), end(pickupNodes), [=](const Vertex* v) {return v->getId() == id;});
        if(it != pickupNodes.end())
            continue;

        return id;
    }
}

vector<Request *> MeatWagons::groupRequests(const int capacity){
    vector <Request *> group;
    int max_dist = 0, dist, pos = 0, max_dist_request_pos = 0;

    auto it = requests.begin();
    Vertex* initial_vert = this->graph->findVertex((*it)->getDest());
    group.push_back((*it));

    it++;
    while(it != requests.end()) {
        if((*it)->isProcessed()) continue;

        Vertex *vert = this->graph->findVertex((*it)->getDest());
        dist = vert->getPosition().euclideanDistance(initial_vert->getPosition());

        if(dist >= this->zoneMaxDist) { it++; continue; };

        if(group.size() < capacity) {
            group.push_back(*it);
            pos++;

            if(dist > max_dist) {
                max_dist = dist;
                max_dist_request_pos = pos;
            }
        }

        else if(dist < max_dist) {
            vector <Request *> groupInit;
            groupInit.assign(group.begin(), group.begin() + max_dist_request_pos);
            vector <Request *> groupFinal;
            groupFinal.assign(group.begin() + max_dist_request_pos + 1, group.end());
            group.clear();
            group.insert(group.end(), groupInit.begin(), groupInit.end());
            group.insert(group.end(), groupFinal.begin(), groupFinal.end());

            max_dist = dist;
            max_dist_request_pos = pos;

            // Check if there is a dist bigger then max_dist in the array
            for(auto itr = 0; itr < group.size(); itr++) {
                auto *vertex = this->graph->findVertex(group[0]->getDest());
                dist = vertex->getPosition().euclideanDistance(initial_vert->getPosition());

                if(dist > max_dist) {
                    max_dist = dist;
                    max_dist_request_pos = itr;
                }
            }

            group.push_back(*it);
        }

        it++;
    }

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
}

int MeatWagons::tspPath(set<Vertex*> &tspNodes, vector<Edge> &tspPath) {
    unordered_set <int> processedEdges, processedInvEdges;
    int totalDist = 0;

    // Because tspNodes is a set ordered by dist -> the first element is the closest to the central
    Vertex *closest = *tspNodes.begin();
    totalDist += this->graph->getPathFromCentralTo(closest->getId(), tspPath);
    tspNodes.erase(tspNodes.begin());

    while(!tspNodes.empty()) {
        Vertex *next = getNearestNeighbour(closest, tspNodes);
        this->graph->dijkstraBidirectional(closest->getId(), next->getId(), processedEdges, processedInvEdges);
        totalDist += this->graph->getPathTo(next->getId(), tspPath);
        tspNodes.erase(next);
        closest = next;
    }

    return totalDist;
}

/**
 * Set the delivery time for each request
 * @param tspPath Edge path of the tsp algorithm
 * @param groupedRequests all the requests that belong to that path
 * @param latestRequest the time of the last request
 * @return
 */
Time MeatWagons::setDeliveriesTime(const vector<Edge> &tspPath, vector<Request *> &groupedRequests, const Time &latestRequest) {
    for(Request *r : groupedRequests) r->setAssigned(false);

    Time totalTime = Time();
    int lastRequestDist = 0;
    bool countLastDist = false;
    int distTillRequest = 0;

    for(Edge edge : tspPath) {
        int nodeId = edge.getDest()->getId();

        // time and dist it takes to cross the road(edge)
        totalTime = totalTime + Time(0, 0, edge.getWeight() / averageVelocity);
        distTillRequest += edge.getWeight();

        if(countLastDist) lastRequestDist += edge.getWeight();
        for(int i = 0; i < groupedRequests.size(); i++) {
            Request *r = *this->requests.find(groupedRequests.at(i));
            if(r->isProcessed()) continue;
            if(r->getAssigned()) continue;
            if(i == groupedRequests.size() - 1) countLastDist = true;
            if(r->getDest() == nodeId) {
                r->setDistFromCentral(distTillRequest);
                r->setRealArrival(latestRequest + totalTime);
                r->setAssigned(true);
                r->setProcessed(true);
            }
        }
    }

    Request *lastRequest = groupedRequests.at(groupedRequests.size() - 1);
    lastRequest->setRealDeliver(lastRequest->getRealArrival() + Time(0, 0, lastRequestDist / averageVelocity));
    Time realDeliver = lastRequest->getRealDeliver();

    for(Request *r : groupedRequests) {
        r->setRealDeliver(realDeliver);
        this->requests.erase(r);
    }

    return totalTime;
}

Delivery* MeatWagons::drawDeliveries(int wagonIndex, int deliveryIndex) {
    if(wagonIndex > this->wagons.size()) return nullptr;

    this->viewer = new GraphVisualizer(600, 600);
    Delivery * delivery = next(this->wagons.begin(), wagonIndex)->getDeliveries().at(deliveryIndex);

    for(auto request : delivery->getRequests()) {
        this->graph->findVertex(request->getDest())->setTag(Vertex::PICKUP);
        this->graph->findVertex(delivery->getDropOff())->setTag(Vertex::DROPOFF);
    }

    vector<int> path = Edge::getIds(delivery->getForwardPath());
    this->viewer->setPath(path, "blue", true);
    this->viewer->draw(this->graph);

    return delivery;
}

/**
 * Iteration: Using a single van with capacity equal to 1 (receive prisoner -> deliver to dropOff location -> return to central)
 */
void MeatWagons::firstIteration() {
    if(wagons.size() != 1)  throw MeatWagonsException("Wrong iteration configuration! Should only be using 1 wagon");
    if(wagons.begin()->getCapacity() != 1)  throw MeatWagonsException("Wrong iteration configuration! Should only use a wagon of capacity = 1");

    unordered_set<int> processedEdges, processedInvEdges;
    for(Wagon wagon : this->wagons) wagon.init();

    auto requestPos = requests.begin();
    while(requestPos != requests.end()) {
        if((*requestPos)->isProcessed()){
            requestPos++;
            continue;
        }

        // requests are ordered by pickup time
        Request *request = *requestPos;
        request->setProcessed(true);
        cout << "requestdest: " << request->getDest() << endl;


        // get wagon
        auto wagonIt = --this->wagons.end();
        auto wagon = *wagonIt;
        this->wagons.erase(wagonIt);

        // central -> dropOff path
        vector<Edge> edgesForwardTrip;
        set<Vertex*> pickupNodes;

        // pickup prisoner path
        int distToPrisoner = this->graph->getPathFromCentralTo(request->getDest(), edgesForwardTrip);
        cout << distToPrisoner << endl;
        cout << "b" << endl;

        // deliver prisoner path
        int dropOffNode = chooseDropOff(pickupNodes);
        this->graph->dijkstraBidirectional(request->getDest(), dropOffNode, processedEdges, processedInvEdges);
        int dropOffDist = graph->getPathTo(dropOffNode, edgesForwardTrip);
        int totalDist = dropOffDist + distToPrisoner;
        cout << "c" << endl;
        // return to central path
        this->graph->dijkstraBidirectional(dropOffNode, central, processedEdges, processedInvEdges);
        totalDist += this->graph->getPathTo(central, edgesForwardTrip);
        cout << "d" << endl;
        // add delivery to wagon and updates request times
        Time lastDeliveryTime = wagon.getDeliveries().size() > 0 ? wagon.getDeliveries().at(wagon.getDeliveries().size() - 1)->getEnd() : request->getArrival() - Time(0, 0, distToPrisoner / averageVelocity);
        request->setRealArrival(lastDeliveryTime + Time(0, 0 , distToPrisoner / averageVelocity));
        request->setRealDeliver(request->getRealArrival() + Time(0, 0, dropOffDist / averageVelocity));

        vector<Request *> vr;
        vr.push_back(request);

        Delivery *delivery = new Delivery(lastDeliveryTime, vr, edgesForwardTrip, totalDist / averageVelocity, dropOffNode);
        wagon.addDelivery(delivery);

        // wagon now is back at the central
        this->wagons.insert(wagon);
        requestPos++;
    }
}

/**
 * Iteration: Using a single van with capacity > 1 (receive prisoner -> deliver to dropOff location -> return to central)
 */
void MeatWagons::secondIteration() {
    if(wagons.size() != 1)  throw MeatWagonsException("Wrong iteration configuration! Should only be using 1 wagon");
    if(wagons.begin()->getCapacity() <= 1)  throw MeatWagonsException("Wrong iteration configuration! Should only use a wagon of capacity = 1");

    unordered_set<int> processedEdges;
    for(Wagon wagon : this->wagons) wagon.init();

    while(!requests.empty()) {
        // get wagon with max capacity (there is only one in this iteration)
        auto wagonIt = --this->wagons.end();
        auto wagon = *wagonIt;
        this->wagons.erase(wagonIt);

        // returned grouped requests and insert tsp nodes
        vector<Request *> groupedRequests = groupRequests(wagon.getCapacity());
        set<Vertex*> tspNodes;
        Request *earliestRequest = groupedRequests.at(0);
        for(auto r : groupedRequests) {
            Vertex *tspNode = this->graph->findVertex(r->getDest());
            tspNodes.insert(tspNode);
            if(r->getArrival() < earliestRequest->getArrival() ) earliestRequest = r;
        }

        // choose drop off node and calculates tsp path
        int dropOffNode = chooseDropOff(tspNodes);
        tspNodes.insert( this->graph->findVertex(dropOffNode));
        vector<Edge> tspPath;
        int totalDist = this->tspPath(tspNodes, tspPath);

        //Time earliestRequestTime = wagon.getDeliveries().size() > 0 ? wagon.getDeliveries().at(wagon.getDeliveries().size() - 1)->getEnd() : earliestRequest->getArrival() - Time(0, 0, distToPrisoner / averageVelocity);

        // gets the time of the latest request (groupedRequests is a set ordered by arrival time)
        //Time totalTime = setDeliveriesTime(tspPath, groupedRequests, latestRequestTime);
        //wagon.setNextAvailableTime(latestRequestTime + totalTime);

        // add delivery to wagon
        //int tripDist = totalDist - groupedRequests.at(0)->getDistFromCentral();
        //Delivery *delivery = new Delivery(latestRequestTime - Time(0, 0, tripDist / averageVelocity), groupedRequests, tspPath, totalDist / averageVelocity, dropOffNode);
        //wagon.addDelivery(delivery);

        // wagon now is back at the central
        //this->wagons.insert(wagon);
    }
}

/**
 * Iteration: Using many vans with capacity > 1 (receive prisoner -> deliver to dropOff location -> return to central)
 */
void MeatWagons::thirdIteration() {
    if(wagons.size() <= 1)  throw MeatWagonsException("Wrong iteration configuration! Should be using more than 1 wagon");

    unordered_set<int> processedEdges;
    for(Wagon wagon : this->wagons) wagon.init();
    Time departureTime = Time();

    while(!requests.empty()) {
        // get wagon with max capacity and the sooner available
        auto wagon = getWagon();
        this->wagons.erase(wagon);

        // returned grouped requests and insert tsp nodes
        vector<Request*> groupedRequests = groupRequests(wagon.getCapacity());
        set<Vertex*> tspNodes;
        departureTime = groupedRequests.at(0)->getArrival();
        for(auto r : groupedRequests) {
            r->setProcessed(true);
            Vertex *tspNode = this->graph->findVertex(r->getDest());
            tspNodes.insert(tspNode);
            if(departureTime < r->getArrival()) departureTime = r->getArrival();
        }

        // choose drop off node and calculates tsp path
        int dropOffNode = chooseDropOff(tspNodes);
        tspNodes.insert( this->graph->findVertex(dropOffNode));
        vector<Edge> tspPath;
        int weight = this->tspPath(tspNodes, tspPath);

        // gets the time of the latest request (groupedRequests is a set ordered by arrival time)
        if(departureTime < wagon.getNextAvailableTime()) departureTime = wagon.getNextAvailableTime();
        Time totalTime = setDeliveriesTime(tspPath, groupedRequests, departureTime);
        wagon.setNextAvailableTime(totalTime);

        // add delivery to wagon
        Delivery *delivery = new Delivery(departureTime, groupedRequests, tspPath, weight, dropOffNode);

        // Get Wagon
        wagon.addDelivery(delivery);
        wagon.setNextAvailableTime(departureTime + totalTime);
    }
}

Wagon MeatWagons::getWagon() const {
    multiset<Wagon>::iterator itr = --this->wagons.end(), it;
    Time min_time = itr->getNextAvailableTime();

    for(itr; itr != wagons.begin(); itr--){
        if(itr->getNextAvailableTime() < min_time){
            it = itr;
            min_time = itr->getNextAvailableTime();
        }
    }

    return *itr;
}
#endif //MEAT_WAGONS_MEATWAGONS_H