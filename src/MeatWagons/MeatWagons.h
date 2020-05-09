#pragma once
#ifndef MEAT_WAGONS_MEATWAGONS_H
#define MEAT_WAGONS_MEATWAGONS_H

#include <unordered_set>
#include <set>
#include <time.h>
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
            for(int i = 0; i < wagons; i++) this->wagons.insert(Wagon(i, 1));
            this->zoneMaxDist = maxDist;
        }

        int getCentral() const;
        void setCentral(const int &id);

        string getGraphName() const;
        void setGraphName(string name);

        Graph* getGraph() const;
        void setGraph(const string path);
        void showGraph();

        void preProcess(int node);
        void shortestPath(int option, int origin, int dest);

        void listWagons() const;
        void addWagon(const int &capacity);
        void removeWagon(const int &id, const int &capacity);

        void listRequests() const;
        void addRequest(const string &prisioner, const int &dest, const int &priority, const Time &arrival);
        void removeRequest(const string &prisioner, const int &dest, const int &priority, const Time &arrival);

        void deliver(int iteration);
        int chooseDropOf(vector<int> const pickupNodes);
        void firstIteration();
        void secondIteration();
        void thirdIteration();
        void calculateCentralVertex() const;

        void drawDeliveries(Delivery delivery);
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
    cout << "Central vertex ID: " << this->getCentral() << endl << endl;
    this->showGraph();
}

void MeatWagons::showGraph() {
    this->viewer = new GraphVisualizer(600, 600);
    this->viewer->draw(this->graph);
}

void MeatWagons::preProcess(int node) {
    if(this->graph == nullptr) throw MeatWagonsException("Graph is null");
    if(!this->graph->preProcess(node)) throw MeatWagonsException("Vertex does not exist");

    int vertex = 0, max = 0, current = 0;

    for(auto it = this->requests.begin(); it != this->requests.end(); it++) {
        if(this->graph->findVertex((*it).getDest()) == nullptr)
            this->requests.erase(*it);
    }

    this->processed = true;
    this->showGraph();
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
    vector<int> vert, edges;
    this->graph->getPathTo(dest, vert, edges);

    // draw shortest path
    this->viewer->setPath(edges, "blue", true);
    this->viewer->draw(this->graph);
}

void MeatWagons::listWagons() const {
    for (auto it = wagons.begin(); it != wagons.end(); it++) {
        cout << "Wagon " << (*it).getId() << ": Capacity " << (*it).getCapacity() << " Available at: "
             << (*it).getNextAvailableTime() << endl << endl;
    }
}

void MeatWagons::addWagon(const int &capacity) {
    wagons.insert(Wagon(wagons.size(), capacity));
}

void MeatWagons::removeWagon(const int &id, const int &capacity) {
    wagons.erase(Wagon(id, capacity));
}

void MeatWagons::listRequests() const {
    for (auto it = requests.begin(); it != requests.end(); it++) {
        cout << "Prisioner " << (*it).getPrisoner() << " to be received in node " << (*it).getDest() << " with priority "
             << (*it).getPriority() << " at " << (*it).getArrival() << " and deliver at " << (*it).getDelivery() << endl << endl;
    }
}


void MeatWagons::addRequest(const string &prisioner, const int &dest, const int &priority, const Time &arrival) {
    Request request = Request(prisioner, dest, priority, arrival);
    requests.insert(request);
}


void MeatWagons::removeRequest(const string &prisioner, const int &dest, const int &priority, const Time &arrival) {
    requests.erase(Request(prisioner, dest, priority, arrival));
}


void MeatWagons::deliver(int iteration) {
    if(this->graph == nullptr) throw MeatWagonsException("Graph is null");
    if(!this->processed) throw MeatWagonsException("Graph was not pre processed");
    if(this->requests.size() == 0) return;

    switch (iteration) {
        case 1: this->firstIteration();
        //case 2: if (!this->graph->dijkstraOrientedSearch(origin, dest)) throw MeatWagonsException("Vertex was not found");break;
        //case 3: if (!this->graph->dijkstraBidirectional(origin, dest)) throw MeatWagonsException("Vertex was not found");break;
        default:
            break;
    }
}

int MeatWagons::chooseDropOf(vector<int> const pickupNodes) {
    srand((unsigned) time(0));
    int random_vertex;
    int id;

    while(true){
        random_vertex = (rand() % graph->getNumVertex());
        id = graph->getVertexSet()[random_vertex]->getId();

        if(find(pickupNodes.begin(), pickupNodes.end(), id) != pickupNodes.end())
            continue;

        return id;
    }
}


// Iteration: Using a single van with capacity equal to 1 (receive prisioner -> deliver to dropOff location -> return to central)
void MeatWagons::firstIteration() {
    if(wagons.size() != 1)  throw MeatWagonsException("Wrong iteration configuration");
    if(wagons.begin()->getCapacity() != 1)  throw MeatWagonsException("Wrong iteration configuration");

    // 1 wagon of capacity = 1
    auto wagonIt = this->wagons.begin();
    auto wagon = *wagonIt;
    this->wagons.erase(wagonIt);
    wagon.init();
    unordered_set<int> processedEdges;

    auto it = requests.begin();
    while(it!= requests.end()) {
        // requests are ordered by pickup time
        Request request = *it;
        it = requests.erase(it);

        // add request for the van (no grouping in this iteration)
        vector<Request*> requests = {&request}; //sclr da probs por causa do pointer

        // central -> dropOff path
        vector<int> nodesForwardTrip, edgesForwardTrip;
        vector<int> pickupNodes;

        // pickup prisioner path
        graph->dijkstra(this->central, request.getDest(), processedEdges);
        int weight = graph->getPathTo(request.getDest(), nodesForwardTrip, edgesForwardTrip);
        pickupNodes.push_back(request.getDest());

        // deliver prisioner path
        int dropOffNode = chooseDropOf(pickupNodes);
        graph->dijkstra(request.getDest(), dropOffNode, processedEdges);
        weight += graph->getPathTo(request.getDest(), nodesForwardTrip, edgesForwardTrip);

        // return to central path
        vector<int> nodesBackwardTrip, edgesBackwardTrip;
        graph->dijkstra(dropOffNode, this->central, processedEdges);  // como e bidirectional sclr basta dar reverse
        weight += graph->getPathTo(request.getDest(), nodesBackwardTrip, edgesBackwardTrip);

        // add delivery to wagon
        Time lastDeliveryTime = wagon.getDeliveries().size() > 0 ? wagon.getDeliveries().at(wagon.getDeliveries().size() - 1)->getEnd() : request.getArrival();
        Delivery *delivery = new Delivery(lastDeliveryTime, requests, edgesForwardTrip, weight);
        wagon.addDelivery(delivery);
    }
    // wagon now is back at the central
    this->wagons.insert(wagon);
}

void MeatWagons::secondIteration() {
}

void MeatWagons::thirdIteration() {
}

void MeatWagons::drawDeliveries(Delivery delivery) {
    this->viewer = new GraphVisualizer(600, 600);

    vector<Wagon> meatWagons (this->wagons.begin(), this->wagons.end());
    this->viewer->setPath(delivery.getForwardPath(), "blue", true);

    for(auto request : delivery.getRequests()) {
        this->viewer->getViewer()->setVertexSize(request->getDest(), 30);
        this->viewer->getViewer()->setVertexColor(request->getDest(), "orange");
    }

    this->viewer->getViewer()->rearrange();
    this->viewer->draw(this->graph);
}

#endif //MEAT_WAGONS_MEATWAGONS_H