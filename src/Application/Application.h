#pragma once
#ifndef MEAT_WAGONS_APPLICATION_H
#define MEAT_WAGONS_APPLICATION_H

#include <iostream>
#include <utility>
#include <sstream>
#include "../Graph/Reader.h"
#include "../GraphViewer/GraphVisualizer.h"

using namespace std;

class AppException : public std::exception {
    public:
        explicit AppException(string  msg) : msg_(std::move(msg)) {}

        [[nodiscard]] string getMessage() const {return(msg_);}
    private:
        string msg_;
};

class Application {
    enum operationType {READ_GRAPH, PRE_PROCESS, SHORTEST_PATH_1, SHORTEST_PATH_2, SHORTEST_PATH_3, SHORTEST_PATH_4,
        TSP_PROBLEM, DISPATCH_POSITION, DD_REQUEST, REMOVE_REQUEST, LIST_REQUESTS, ADD_WAGON, REMOVE_WAGON,
        LIST_WAGONS, GRAPH_CONNECTIVITY, INTERESTS_POINTS_CONNECTIVITY};

    private:
        GraphVisualizer *viewer = nullptr;
        Graph *graph = nullptr;

        operationType operation;
        vector<int> operands;
        string graphPath;

    public:
        Application(char *graphPath) {
            this->graphPath = graphPath;
        }

        static void usage();
        bool start();
        void run();
};

void Application::usage() {
    cout << "Menu Options:" << endl;
    cout << "readGraph <folder path>" << endl;
    cout << "preProcess <node id>" << endl;
    cout << "shortestPath dijkstra <origin node>" << endl;
    cout << "shortestPath dijkstra <origin node> <destination node>" << endl;
    cout << "shortestPath dijkstraOriented <origin node> <destination node>" << endl;
    cout << "shortestPath dijkstraBidirectional <origin node> <destination node>" << endl;
    cout << "shortestPath dijkstraOrientedBidirectional <origin node> <destination node>" << endl;
    cout << "exit" << endl;
}

bool Application::start() {
    string l, operation;

    usage();

    getline(cin, l);
    stringstream line(l);

    line >> operation;

    if (operation == "exit") return false;

    else if (operation == "readGraph") {
        string fileName;
        if (!(line >> fileName)) throw AppException("Incorrect number of parameters");
        this->operation = READ_GRAPH;
        this->graphPath = fileName;
    } else if (operation == "preProcess") {
        int vertex;
        if (!(line >> vertex)) throw AppException("Incorrect number of parameters");
        this->operation = PRE_PROCESS;
        this->operands.push_back(vertex);
    } else if (operation == "shortestPath") {
        string variant;
        int vertex1, vertex2;

        if (!(line >> variant)) throw AppException("Incorrect number of parameters");
        if (!(line >> vertex1)) throw AppException("Incorrect number of parameters");

        if (variant == "dijkstra") {
            if (!(line >> vertex2)) {
                this->operation = SHORTEST_PATH_1;
                this->operands.push_back(vertex1);
            } else {
                this->operation = SHORTEST_PATH_1;
                this->operands.push_back(vertex1);
                this->operands.push_back(vertex2);
            }
        } else if (variant == "dijkstraOriented") {
            if (!(line >> vertex2)) throw AppException("Incorrect number of parameters");
            this->operation = SHORTEST_PATH_2;
            this->operands.push_back(vertex1);
            this->operands.push_back(vertex2);
        } else if (variant == "dijkstraBidirectional") {
            if (!(line >> vertex2)) throw AppException("Incorrect number of parameters");
            this->operation = SHORTEST_PATH_3;
            this->operands.push_back(vertex1);
            this->operands.push_back(vertex2);
        } else if (variant == "dijkstraOrientedBidirectional") {
            if (!(line >> vertex2)) throw AppException("Incorrect number of parameters");
            this->operation = SHORTEST_PATH_4;
            this->operands.push_back(vertex1);
            this->operands.push_back(vertex2);
        }
    }

    return true;
}

void Application::run() {
    start();

    switch (this->operation) {
        case READ_GRAPH: {
            Reader graphReader = Reader(graphPath);
            graph = graphReader.read();

            viewer = new GraphVisualizer(600, 600);
            viewer->draw(graph);

            break;
        }

        case PRE_PROCESS: {
            if(graph == nullptr)
                throw AppException("You must read the graph firstly, before running this operation");

            Vertex *origin = graph->findVertex(operands.at(1));
            origin != nullptr ? graph->removeUnvisited(origin) : throw AppException("vertex does not exist");

            viewer = new GraphVisualizer(600, 600);
            viewer->draw(graph);
            break;
        }

        case SHORTEST_PATH_1: {
            if (this->graph == nullptr)
                throw AppException("You must read the graph firstly, before running this operation");

            if(operands.size() == 1)
                if (!graph->dijkstraSingleSource(operands.at(1)))
                    throw AppException("Vertex not found");
            else
                if (!graph->dijkstraSingleSource(operands.at(1), operands.at(2)))
                    throw AppException("One of the Vertexes was not found");

            viewer = new GraphVisualizer(600, 600);
            viewer->draw(graph);

            break;
        }

        case SHORTEST_PATH_2: {
            if (this->graph == nullptr)
                throw AppException("You must read the graph firstly, before running this operation");

            break;
        }

        case SHORTEST_PATH_3: {
            if (this->graph == nullptr)
                throw AppException("You must read the graph firstly, before running this operation");

            break;
        }

        case SHORTEST_PATH_4: {
            if (this->graph == nullptr)
                throw AppException("You must read the graph firstly, before running this operation");

            break;
        }

        default:
            break;
    }
}


#endif //MEAT_WAGONS_APPLICATION_H