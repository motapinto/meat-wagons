#pragma once
#ifndef MEAT_WAGONS_APPLICATION_H
#define MEAT_WAGONS_APPLICATION_H

#include <iostream>
#include <utility>
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
        vector<string> operands;

    public:
        Application(int argc, char* argv[]) {
            if(argc < 2) throw AppException("Incorrect number of parameters");

            if(strcmp(argv[1], "readGraph") == 0) {
                if(argc < 3) throw AppException("Incorrect number of parameters");
                this->operation = READ_GRAPH;
                this->operands.emplace_back(argv[2]);
            }

            else if(strcmp(argv[1], "preProcess") == 0) {
                if(argc < 3) throw AppException("Incorrect number of parameters");
                this->operation = PRE_PROCESS;
                this->operands.emplace_back(argv[2]);
            }

            else if(strcmp(argv[1], "shortestPath") == 0) {

                if(strcmp(argv[2], "dijkstra") == 0) {
                    if(argc < 4) throw AppException("Incorrect number of parameters");
                    this->operation = SHORTEST_PATH_1;
                    this->operands.emplace_back(argv[3]);
                }

                else if(strcmp(argv[1], "dijkstra") == 0) {
                    if(argc < 5) throw AppException("Incorrect number of parameters");
                    this->operation = SHORTEST_PATH_1;
                    this->operands.emplace_back(argv[3]);
                    this->operands.emplace_back(argv[4]);
                }

                else if(strcmp(argv[1], "dijkstraOriented") == 0) {
                    if(argc < 5) throw AppException("Incorrect number of parameters");
                    this->operation = SHORTEST_PATH_2;
                    this->operands.emplace_back(argv[3]);
                    this->operands.emplace_back(argv[4]);
                }

                else if(strcmp(argv[1], "dijkstraBidirectional") == 0) {
                    if(argc < 5) throw AppException("Incorrect number of parameters");
                    this->operation = SHORTEST_PATH_3;
                    this->operands.emplace_back(argv[3]);
                    this->operands.emplace_back(argv[4]);
                }

                else if(strcmp(argv[1], "dijkstraOrientedBidirectional") == 0) {
                    if(argc < 5) throw AppException("Incorrect number of parameters");
                    this->operation = SHORTEST_PATH_4;
                    this->operands.emplace_back(argv[3]);
                    this->operands.emplace_back(argv[4]);
                }
            }

            else
                usage();
        }

        static void usage();
        void run();
};

void Application::usage() {
    cout << "Usage:" << endl;
    cout << "./meat-wagons readGraph <folder path>" << endl;
    cout << "./meat-wagons preProcess <node id>" << endl;
    cout << "./meat-wagons shortestPath dijkstra <origin node>" << endl;
    cout << "./meat-wagons shortestPath dijkstra <origin node> <destination node>" << endl;
    cout << "./meat-wagons shortestPath dijkstraOriented <origin node> <destination node>" << endl;
    cout << "./meat-wagons shortestPath dijkstraBidirectional <origin node> <destination node>" << endl;
    cout << "./meat-wagons shortestPath dijkstraOrientedBidirectional <origin node> <destination node>" << endl;
}

void Application::run() {
    switch(this->operation) {
        case READ_GRAPH: {
            Reader graphReader = Reader(operands.at(0));
            graph = graphReader.read();

            viewer = new GraphVisualizer(600, 600);
            viewer->draw(graph);

            break;
        }

        case PRE_PROCESS: {
            if(graph == nullptr)
                throw AppException("You must read the graph firstly, before running this operation");

            Vertex *origin = graph->findVertex(stoi(operands.at(1)));
            graph->removeUnvisited(origin);

            viewer = new GraphVisualizer(600, 600);
            viewer->draw(graph);
            break;
        }

        case SHORTEST_PATH_1: {
            if(this->graph == nullptr)
                throw AppException("You must read the graph firstly, before running this operation");

            break;
        }

        case SHORTEST_PATH_2: {
            if(this->graph == nullptr)
                throw AppException("You must read the graph firstly, before running this operation");

            break;
        }

        case SHORTEST_PATH_3: {
            if(this->graph == nullptr)
                throw AppException("You must read the graph firstly, before running this operation");

            break;
        }

        case SHORTEST_PATH_4: {
            if(this->graph == nullptr)
                throw AppException("You must read the graph firstly, before running this operation");

            break;
        }

        default:
            break;
    }
}

#endif //MEAT_WAGONS_APPLICATION_H