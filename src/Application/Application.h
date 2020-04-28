#pragma once
#ifndef MEAT_WAGONS_APPLICATION_H
#define MEAT_WAGONS_APPLICATION_H

#include <fstream>
#include "../Graph/Reader.h"
#include "../MeatWagons/Wagon.h"
#include "../GraphViewer/GraphVisualizer.h"

class AppException : public std::exception {
    public:
        explicit AppException(string  msg) : msg_(std::move(msg)) {}

        [[nodiscard]] string getMessage() const {return(msg_);}
    private:
        string msg_;
};

class Application {
    enum operationType {READ_GRAPH, PRE_PROCESS, SHORTEST_PATH_1, SHORTEST_PATH_2, SHORTEST_PATH_3, SHORTEST_PATH_4,
        TSP_PROBLEM, ADD_REQUEST, REMOVE_REQUEST, LIST_REQUESTS, ADD_WAGON, REMOVE_WAGON,
        LIST_WAGONS, GRAPH_CONNECTIVITY, INTERESTS_POINTS_CONNECTIVITY};

    private:
        GraphVisualizer *viewer = nullptr;
        Graph *graph = nullptr;

        operationType operation;
        vector<int> operands;
        string graphPath;
        int central = 90379359; //central node id - so funciona para o PORTO de pois mudar isto para outros mapas (fazer script com pontos intereresse)

    public:
        Application(char *graphPath) {
            this->graphPath = graphPath;

            Reader graphReader = Reader(this->graphPath);
            graph = graphReader.read();

            //viewer = new GraphVisualizer(graph->getWidth(), graph->getHeight());
           //viewer->draw(graph);
        }

        static void usage();
        bool start();
        void run();
};

void Application::usage() {
    cout << "\tMenu Options:" << endl;
    cout << "\treadGraph <graph folder path>" << endl;
    cout << "\tpreProcess {<node id>}" << endl;
    cout << "\tshortestPath dijkstra <origin node>" << endl;
    cout << "\tshortestPath {dijkstra, dijkstraOriented, dijkstraBidirectional, dijkstraOrientedBidirectional} <origin node> <destination node>" << endl;
    cout << "\tsetCentral <node id>"<< endl;
    cout << "\tdeliver <requests folder path>"<< endl;
    cout << "\taddRequest <requests folder path>"<< endl;
    cout << "\tremoveRequest <requests folder path>"<< endl;
    cout << "\tlistRequests <requests folder path>"<< endl;

    cout << "\texit" << endl << endl;
    cout << "Input:  ";
}

bool Application::start() {
    string l, operation;

    usage();

    cin.clear();

    getline(cin, l);
    stringstream line(l);

    line >> operation;

    if (operation == "exit") return false;

    else if (operation == "readGraph") {
        string fileName;
        if (!(line >> fileName)) throw AppException("Incorrect number of parameters");
        this->operation = READ_GRAPH;
        this->graphPath = fileName;
    }

    else if (operation == "preProcess") {
        int vertex;
        this->operation = PRE_PROCESS;
        if (!(line >> vertex))
            this->operands.push_back(central);
        else
            this->operands.push_back(vertex);
    }

    else if (operation == "shortestPath") {
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
        }
        else if (variant == "dijkstraOriented") {
            if (!(line >> vertex2)) throw AppException("Incorrect number of parameters");
            this->operation = SHORTEST_PATH_2;
            this->operands.push_back(vertex1);
            this->operands.push_back(vertex2);
        }
        else if (variant == "dijkstraBidirectional") {
            if (!(line >> vertex2)) throw AppException("Incorrect number of parameters");
            this->operation = SHORTEST_PATH_3;
            this->operands.push_back(vertex1);
            this->operands.push_back(vertex2);
        }
        else if (variant == "dijkstraOrientedBidirectional") {
            if (!(line >> vertex2)) throw AppException("Incorrect number of parameters");
            this->operation = SHORTEST_PATH_4;
            this->operands.push_back(vertex1);
            this->operands.push_back(vertex2);
        }
    }

    else if(operation == "setCentral") {
        int centralId;
        if (!(line >> centralId)) throw AppException("Incorrect number of parameters");
        this->central = centralId;
    }

    else if(operation == "deliver") {

    } else if(operation == "addRequest") {

    } else if(operation == "removeRequest") {

    } else if(operation == "listRequests") {

    } else if(operation == "addWagon") {

    } else if(operation == "removeWagon") {

    } else if(operation == "listWagons") {

    }

    cout << endl;
    return true;
}

void Application::run() {
    start();

    switch (this->operation) {
        case READ_GRAPH: {
            Reader graphReader = Reader(graphPath);
            graph = graphReader.read();

            //viewer = new GraphVisualizer(600, 600);
            //viewer->draw(graph);
            break;
        }

        case PRE_PROCESS: {
            if(graph == nullptr)
                throw AppException("You must read the graph firstly, before running this operation");

            //if(!graph->preProcess(operands.at(0))) throw AppException("Vertex does not exist");
            if(!graph->preProcess(90379359)) throw AppException("Vertex does not exist");

            viewer = new GraphVisualizer(graph->getWidth(), graph->getHeight());
            viewer->draw(graph);
            break;
        }

        case SHORTEST_PATH_1: {
            if (this->graph == nullptr)
                throw AppException("You must read the graph firstly, before running this operation");

            char proceed;
            //cout << "Note that if you want to execute this algorithm with the pre processed graph you should have done that first" << endl;
            //cout << "Proceed(Y/N)?: ";
            //if((cin>>proceed) && proceed != 'Y') break;

            if(operands.size() == 1) {
                if (!graph->dijkstraSingleSource(operands.at(0)))
                    throw AppException("Vertex not found");
            } else {
                if (!graph->dijkstra(operands.at(0), operands.at(1)))
                    throw AppException("One of the Vertexes was not found");
            }

            vector<int> vert, edges;

            graph->getPathTo(operands.at(1), vert, edges); //isto esta mal.... fazer get path to no caso de receber so 1 operando


            viewer = new GraphVisualizer(600, 600);
            viewer->setPath(vert, edges);
            cout << vert.size() << " " << edges.size() << " " << endl;
            viewer->draw(graph);

            break;
        }

        case SHORTEST_PATH_2: {
            if (this->graph == nullptr)
                throw AppException("You must read the graph firstly, before running this operation");

            char proceed;
            cout << "Note that if you want to execute this algorithm with the pre processed graph you should have done that first" << endl;
            cout << "Proceed(Y/N)?: ";
            if((cin>>proceed) && proceed != 'Y') break;

            if (!graph->dijkstraOrientedSearch(operands.at(0), operands.at(1)))
                throw AppException("One of the Vertexes was not found");

            vector<int> vert, edges;

            graph->getPathTo(operands.at(1), vert, edges);


            viewer = new GraphVisualizer(600, 600);
            viewer->setPath(vert, edges);
            viewer->draw(graph);

            break;
        }

        case SHORTEST_PATH_3: {
            if (this->graph == nullptr)
                throw AppException("You must read the graph firstly, before running this operation");

            char proceed;
            cout << "Note that if you want to execute this algorithm with the pre processed graph you should have done that first" << endl;
            cout << "Proceed(Y/N)?: ";
            if((cin>>proceed) && proceed != 'Y') break;

            if (!graph->dijkstraBidirectional(operands.at(0), operands.at(1))) //make dijkstraBidirectional withouth a*
                throw AppException("One of the Vertexes was not found");

            vector<int> vert, edges;

            graph->getPathTo(operands.at(1), vert, edges);


            viewer = new GraphVisualizer(600, 600);
            viewer->setPath(vert, edges);
            viewer->draw(graph);

            break;
        }

        case SHORTEST_PATH_4: {
            if (this->graph == nullptr)
                throw AppException("You must read the graph firstly, before running this operation");

            char proceed;
            cout << "Note that if you want to execute this algorithm with the pre processed graph you should have done that first" << endl;
            cout << "Proceed(Y/N)?: ";
            if((cin>>proceed) && proceed != 'Y') break;

            if (!graph->dijkstraBidirectional(operands.at(0), operands.at(1)))
                throw AppException("One of the Vertexes was not found");

            vector<int> vert, edges;

            graph->getPathTo(operands.at(1), vert, edges);


            viewer = new GraphVisualizer(600, 600);
            viewer->setPath(vert, edges);
            viewer->draw(graph);

            break;
        }

        default:
            break;
    }
}


#endif //MEAT_WAGONS_APPLICATION_H