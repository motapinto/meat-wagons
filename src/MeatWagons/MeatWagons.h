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

bool compareRequests(Request* r1, Request* r2){
    return r1->getArrival() < r2->getArrival();
}

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
        multiset<Wagon> wagons;
        multiset<Request*> requests;
        int zoneMaxDist;
        bool processed = false;
        const static int averageVelocity = 9;

    public:
        MeatWagons(const int wagons) {
            for(int i = 0; i < wagons; i++){
                this->wagons.insert(Wagon(i, 4));
            }
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
        multiset<Wagon>::iterator getWagon();

        void listRequests() const;

        void deliver(const int iteration);
        int chooseDropOff(const vector<Vertex*> &pickupNodes);
        vector<Request*> groupRequests(const int capacity);
        Vertex* getNearestNeighbour(Vertex *node,  const vector<Vertex*> &neighbours);
        int tspPath(vector<Vertex*> &tspNodes, vector<Request *> requests, vector<Edge> &tspPath, int dropOffNode, Time& startTime);
        Time setDeliveriesTime(const vector<Edge> &tspPath, vector<Request *> &groupedRequests, const Time &latestRequest);
        Delivery* drawDeliveries(int wagonIndex, int deliveryIndex);
        Request* findRequest(Vertex * vert, vector<Request *> requests);
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

int MeatWagons::chooseDropOff(const vector<Vertex*> &pickupNodes) {
    srand((unsigned) time(0));
    int id, randomId;

    while(true){
        randomId = (rand() % this->requests.size());
        id = this->pointsOfInterest.at(randomId)->getId();

        auto it = find_if(begin(pickupNodes), end(pickupNodes), [=](const Vertex* v) {
            return v->getId() == id && (find(begin(pickupNodes), end(pickupNodes), v) != pickupNodes.end());
        });

        if(it != pickupNodes.end()) continue;
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

Vertex* MeatWagons::getNearestNeighbour(Vertex *node,  const vector<Vertex*> &neighbours) {
    double nearestDistance = this->graph->findVertex(node->getId())->getPosition().euclideanDistance((*neighbours.begin())->getPosition());
    auto nearestNeighbour = *neighbours.begin();

    for(auto it = ++neighbours.begin(); it != neighbours.end(); it++) {
        auto currDistance = this->graph->findVertex(node->getId())->getPosition().euclideanDistance((*it)->getPosition());
        if(currDistance < nearestDistance) {
            nearestDistance = currDistance;
            nearestNeighbour = *it;
        }
    }

    return nearestNeighbour;
}

Request *MeatWagons::findRequest(Vertex * vert, vector<Request *> requests) {
    for (auto it = requests.begin(); it != requests.end(); it++) {
        if ((*it)->getDest() == vert->getId()) {
            return *it;
        }
    }

    return nullptr;
}

int MeatWagons::tspPath(vector<Vertex*> &tspNodes, vector<Request *> requests, vector<Edge> &tspPath, int dropOffNode, Time& startTime){
    unordered_set <int> processedEdges, processedInvEdges;
    int totalDist = 0;

    // Because tspNodes is a set ordered by dist -> the first element is the closest to the central
    Vertex *closest = *tspNodes.begin();
    totalDist += this->graph->getPathFromCentralTo(closest->getId(), tspPath);
    tspNodes.erase(tspNodes.begin());
    Request* r = findRequest(closest, requests);
    if(r != nullptr){
        if(startTime == r->getArrival()) {
            startTime = startTime - Time(0, 0, totalDist / averageVelocity);
        }
        r->setDistFromCentral(totalDist);
        r->setRealArrival(startTime + Time(0, 0, totalDist / averageVelocity));
    }

    while(!tspNodes.empty()) {
        Vertex *next = getNearestNeighbour(closest, tspNodes);
        this->graph->dijkstra(closest->getId(), next->getId(), processedEdges);
        totalDist += this->graph->getPathTo(next->getId(), tspPath);
        r = findRequest(next, requests);
        if(r != nullptr){
            r->setDistFromCentral(totalDist);
            r->setRealArrival(startTime + Time(0, 0, totalDist / averageVelocity));
        }
        
        tspNodes.erase(std::find(tspNodes.begin(), tspNodes.end(), next));
        closest = next;
    }

    this->graph->dijkstraBidirectional(closest->getId(), dropOffNode, processedEdges, processedInvEdges);
    totalDist += this->graph->getPathTo(dropOffNode, tspPath);

    for(Request * r : requests){
        r->setRealDeliver(startTime + Time(0, 0, totalDist / averageVelocity));
        this->requests.erase(r);
    }

    return totalDist;
}

Delivery* MeatWagons::drawDeliveries(int wagonIndex, int deliveryIndex) {
    if(wagonIndex > this->wagons.size()) return nullptr;

    this->viewer = new GraphVisualizer(600, 600);
    Delivery * delivery = next(this->wagons.begin(), wagonIndex)->getDeliveries().at(deliveryIndex);

    for(auto request : delivery->getRequests()) {
        stringstream stream;
        stream << request->getRealArrival();
        this->graph->findVertex(request->getDest())->setTag(Vertex::PICKUP);
        this->viewer->getViewer()->setVertexLabel(request->getDest(), request->getPrisoner() + " arrival at: " + stream.str());
        this->graph->findVertex(delivery->getDropOff())->setTag(Vertex::DROPOFF);
    }

    vector<int> path = Edge::getIds(delivery->getForwardPath());
    this->viewer->setPath(path, "blue", true);
    this->viewer->draw(this->graph);

    for(auto request : delivery->getRequests()) {
        this->graph->findVertex(request->getDest())->setTag(Vertex::INTEREST_POINT);
        this->graph->findVertex(delivery->getDropOff())->setTag(Vertex::INTEREST_POINT);
    }

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

        // get wagon
        auto wagonIt = --this->wagons.end();
        auto wagon = *wagonIt;
        this->wagons.erase(wagonIt);

        // central -> dropOff path
        vector<Edge> edgesForwardTrip;

        vector<Vertex*> pickupNodes;


        // pickup prisoner path
        int distToPrisoner = this->graph->getPathFromCentralTo(request->getDest(), edgesForwardTrip);

        // deliver prisoner path
        int dropOffNode = chooseDropOff(pickupNodes);
        this->graph->dijkstraBidirectional(request->getDest(), dropOffNode, processedEdges, processedInvEdges);
        int dropOffDist = graph->getPathTo(dropOffNode, edgesForwardTrip);
        int totalDist = dropOffDist + distToPrisoner;

        // return to central path
        this->graph->dijkstraBidirectional(dropOffNode, central, processedEdges, processedInvEdges);
        totalDist += this->graph->getPathTo(central, edgesForwardTrip);

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
        sort(groupedRequests.begin(), groupedRequests.end(), compareRequests);
        vector<Vertex*> tspNodes;
        Request *earliestRequest = groupedRequests.at(0);
        for(auto r : groupedRequests) {
            Vertex *tspNode = this->graph->findVertex(r->getDest());
            tspNodes.push_back(tspNode);
        }

        Time startTime = wagon.getDeliveries().size() > 0 ? wagon.getDeliveries().at(wagon.getDeliveries().size() - 1)->getEnd() : groupedRequests[0]->getArrival();


        // choose drop off node and calculates tsp path
        int dropOffNode = chooseDropOff(tspNodes);
        vector<Edge> tspPath;

        int totalDist = this->tspPath(tspNodes, groupedRequests, tspPath, dropOffNode, startTime);


        // gets the time of the latest request (groupedRequests is a set ordered by arrival time)
        wagon.setNextAvailableTime(startTime + Time(0, 0, (totalDist * 2) / averageVelocity));

        // add delivery to wagon
        Delivery *delivery = new Delivery(startTime, groupedRequests, tspPath, (totalDist * 2) / averageVelocity, dropOffNode);

        wagon.addDelivery(delivery);

        // wagon now is back at the central
        this->wagons.insert(wagon);
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
        auto wagonIt = getWagon();
        auto wagon = *wagonIt;
        this->wagons.erase(wagonIt);

        // returned grouped requests and insert tsp nodes
        vector<Request *> groupedRequests = groupRequests(wagon.getCapacity());
        sort(groupedRequests.begin(), groupedRequests.end(), compareRequests);
        vector<Vertex*> tspNodes;
        Request *earliestRequest = groupedRequests.at(0);
        for(auto r : groupedRequests) {
            Vertex *tspNode = this->graph->findVertex(r->getDest());
            tspNodes.push_back(tspNode);
        }

        Time startTime = wagon.getDeliveries().size() > 0 ? wagon.getDeliveries().at(wagon.getDeliveries().size() - 1)->getEnd() : groupedRequests[0]->getArrival();


        // choose drop off node and calculates tsp path
        int dropOffNode = chooseDropOff(tspNodes);
        vector<Edge> tspPath;

        int totalDist = this->tspPath(tspNodes, groupedRequests, tspPath, dropOffNode, startTime);


        // gets the time of the latest request (groupedRequests is a set ordered by arrival time)
        wagon.setNextAvailableTime(startTime + Time(0, 0, (totalDist * 2) / averageVelocity));

        // add delivery to wagon
        Delivery *delivery = new Delivery(startTime, groupedRequests, tspPath, (totalDist * 2) / averageVelocity, dropOffNode);

        wagon.addDelivery(delivery);

        wagons.insert(wagon);
    }
}

multiset<Wagon>::iterator MeatWagons::getWagon(){
    multiset<Wagon>::iterator itr = --this->wagons.end(), it = itr;
    Time min_time = itr->getNextAvailableTime();

    for(itr; itr != wagons.begin(); itr--){
        if(itr->getNextAvailableTime() == Time()){
            return itr;
        }

        if(itr->getNextAvailableTime() < min_time){
            it = itr;
            min_time = itr->getNextAvailableTime();
        }
    }

    if(wagons.begin()->getNextAvailableTime() <= Time()){
        return wagons.begin();
    }

    if(wagons.begin()->getNextAvailableTime() < min_time){
        return wagons.begin();
    }

    return it;
}


#endif //MEAT_WAGONS_MEATWAGONS_H