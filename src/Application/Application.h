#pragma once
#ifndef MEAT_WAGONS_APPLICATION_H
#define MEAT_WAGONS_APPLICATION_H

#include <iostream>
#include <utility>
#include "../Graph/Reader.h"

using namespace std;

class AppException : public std::exception {

public:
    explicit AppException(string  msg) : msg_(std::move(msg)) {}
    ~AppException() override = default;

    [[nodiscard]] string getMessage() const {return(msg_);}
private:
    string msg_;
};

class Application {
    enum operationType {READ_GRAPH, PRE_PROCESS, SHORTEST_PATH_1, SHORTEST_PATH_2, SHORTEST_PATH_3, SHORTEST_PATH_4,
        TSP_PROBLEM, DISPATCH_POSITION, DD_REQUEST, REMOVE_REQUEST, LIST_REQUESTS, ADD_WAGON, REMOVE_WAGON,
        LIST_WAGONS, GRAPH_CONNECTIVITY, INTERESTS_POINTS_CONNECTIVITY};

    private:
        operationType operation;
        vector<string> operands;
        Graph *graph;

    public:
        Application(int argc, char* argv[]) {
            if(argc < 2) throw AppException("Incorrect number of parameters");

            if((string)argv[1] == "readGraph") {
                if(argc < 3) throw AppException("Incorrect number of parameters");
                this->operation = READ_GRAPH;
                this->operands.emplace_back(argv[2]);
            }

            else if((string)argv[1] == "preProcess") {
                if(argc < 3) throw AppException("Incorrect number of parameters");
                this->operation = PRE_PROCESS;
                this->operands.emplace_back(argv[2]);
            }

            else if((string)argv[1] == "shortestPath") {

                if(argv[2] == "dijkstra") {
                    if(argc < 4) throw AppException("Incorrect number of parameters");
                    this->operation = SHORTEST_PATH_1;
                    this->operands.emplace_back(argv[3]);
                }

                else if(argv[1] == "dijkstra") {
                    if(argc < 5) throw AppException("Incorrect number of parameters");
                    this->operation = SHORTEST_PATH_1;
                    this->operands.emplace_back(argv[3]);
                    this->operands.emplace_back(argv[4]);
                }

                else if(argv[1] == "dijkstraOriented") {
                    if(argc < 5) throw AppException("Incorrect number of parameters");
                    this->operation = SHORTEST_PATH_2;
                    this->operands.emplace_back(argv[3]);
                    this->operands.emplace_back(argv[4]);
                }

                else if(argv[1] == "dijkstraBidirectional") {
                    if(argc < 5) throw AppException("Incorrect number of parameters");
                    this->operation = SHORTEST_PATH_3;
                    this->operands.emplace_back(argv[3]);
                    this->operands.emplace_back(argv[4]);
                }

                else if(argv[1] == "dijkstraOrientedBidirectional") {
                    if(argc < 5) throw "Incorrect number of parameters";
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
            //Reader graphReader = Reader(this->operands.at(0));
            //this->graph = graphReader.read();
            break;
        }

        case PRE_PROCESS: {
            if(this->graph == nullptr)
                throw AppException("You must read the graph firstly, before running this operation");

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