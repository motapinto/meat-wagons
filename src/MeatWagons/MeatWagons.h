#pragma once
#ifndef MEAT_WAGONS_MEATWAGONS_H
#define MEAT_WAGONS_MEATWAGONS_H

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
        void readRequests(string requestsPath);
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

    if(!graphReader.readGraph(graphRead, central, pointsOfInterest)) throw MeatWagonsException("Graph is null");
    readRequests(graphPath);

    this->graph = graphRead;
    this->showGraph();
}

void MeatWagons::readRequests(string requestsPath) {
    Reader graphReader = Reader(requestsPath);
    if(!graphReader.readRequests(requests)) throw MeatWagonsException("Graph is null");
}

void MeatWagons::showGraph() {
    this->viewer = new GraphVisualizer(600, 600);
    this->viewer->draw(this->graph);
}

void MeatWagons::preProcess(int node) {
    if(this->graph == nullptr) throw MeatWagonsException("Graph is null");
    if(!this->graph->preProcess(node)) throw MeatWagonsException("Vertex does not exist");

    for(auto it = this->requests.begin(); it != this->requests.end(); it++) {
        if(this->graph->findVertex((*it).getDest()) == nullptr)
            this->requests.erase(*it);
    }

    this->showGraph();
}

void MeatWagons::shortestPath(int option, int origin, int dest) {
    if(this->graph == nullptr) throw MeatWagonsException("Graph is null");

    switch (option) {
        case 1: if (!this->graph->dijkstra(origin, dest)) throw MeatWagonsException("Vertex not found"); break;
        case 2: if (!this->graph->dijkstraOrientedSearch(origin, dest)) throw MeatWagonsException("Vertex was not found");break;
        case 3: if (!this->graph->dijkstraBidirectional(origin, dest)) throw MeatWagonsException("Vertex was not found");break;
    }

    // get path
    vector<int> vert, edges;
    this->graph->getPathTo(dest, vert, edges);

    // draw path
    this->viewer = new GraphVisualizer(600, 600);
    this->viewer->setPath(vert, edges);
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



/*void MeatWagons::deliver(int iteration) {
    if(this->graph == nullptr) throw MeatWagonsException("Graph is null");
    if(this->requests.size() == 0) throw MeatWagonsException("No requests to process");

    switch (iteration) {
        case 1: this->firstIteration();
        //case 2: if (!this->graph->dijkstraOrientedSearch(origin, dest)) throw MeatWagonsException("Vertex was not found");break;
        //case 3: if (!this->graph->dijkstraBidirectional(origin, dest)) throw MeatWagonsException("Vertex was not found");break;
    }
}
*/
int MeatWagons::chooseDropOf(vector<int> const pickupNodes) {
    srand((unsigned) time(0));
    int random_vertex;
    int id;
    while(true){
        random_vertex = (rand() % graph->getNumVertex());
        id = graph->getVertexSet()[random_vertex]->getId();

        if(find(pickupNodes.begin(), pickupNodes.end(), id) != pickupNodes.end()){
            continue;
        }

        return id;
    }

}

/*
// Iteration: Using a single van with capacity equal to 1
// Receive prisioner
// Deliver to a random point of interest(not the pickup)
// Get back to central to process another request
void MeatWagons::firstIteration() {
    if(graph == nullptr) throw MeatWagonsException("Graph is null");
    if(wagons.size() != 1)  throw MeatWagonsException("Wrong iteration configuration");
    if(wagons.begin()->getCapacity() != 1)  throw MeatWagonsException("Wrong iteration configuration");

    for(auto it = requests.begin(); it!= requests.end(); it++) {
        Request request = *it;
        it = requests.erase(it);

        // pickup prisioner
        graph->dijkstra(central, request.getDest());
        vector<int> nodesPickUp, edgesPickUp;
        graph->getPathTo(request.getDest(), nodesPickUp, edgesPickUp);

        // deliver prisioner
        vector<int> pickupNodes;
        pickupNodes.push_back(request.getDest());
        graph->dijkstra(request.getDest(), chooseDropOf(pickupNodes));
        vector<int> nodesDelivery, edgesDelivery;
        graph->getPathTo(request.getDest(), nodesDelivery, edgesDelivery);

        // return to central


    }
}

void MeatWagons::secondIteration() {
}

void MeatWagons::thirdIteration() {
}*/

#endif //MEAT_WAGONS_MEATWAGONS_H