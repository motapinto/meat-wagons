#pragma once
#ifndef MEAT_WAGONS_APPLICATION_H
#define MEAT_WAGONS_APPLICATION_H

#include <fstream>
#include "../MeatWagons/MeatWagons.h"

class AppException : public std::exception {
public:
    explicit AppException(string  msg) : msg_(std::move(msg)) {}

    [[nodiscard]] string getMessage() const {return(msg_);}
private:
    string msg_;
};

class Application {
    private:
        MeatWagons *controller = new MeatWagons(10, 100);
        GraphVisualizer *viewer = nullptr;

    public:
        static void usage();
        bool run();
};

void Application::usage() {
    cout << "\tMenu Options:" << endl;
    cout << "\treadGraph <graph folder path>" << endl;
    cout << "\tpreProcess {<node id>}" << endl;
    cout << "\tshortestPath {dijkstra, dijkstraOriented, dijkstraBidirectional} <origin node> <destination node>" << endl;
    cout << "\tsetCentral <node id>"<< endl;
    cout << "\tdeliver <requests folder path>"<< endl;
    cout << "\taddRequest <requests folder path>"<< endl;
    cout << "\tremoveRequest <requests folder path>"<< endl;
    cout << "\tlistRequests <requests folder path>"<< endl;

    cout << "\texit" << endl << endl;
    cout << "Input:  ";
}

bool Application::run() {
    string l, operation;

    usage();

    cin.clear();

    getline(cin, l);
    stringstream line(l);

    line >> operation;

    if (operation == "exit") return false;

    else if (operation == "readGraph") {
        string fileName;
        if (!(line >> fileName)) controller->setGraph("maps/PortugalMaps/Porto");
        else controller->setGraph(fileName);

        auto *viewer = new GraphVisualizer(600, 600);
        viewer->draw(controller->getGraph());
    }

    else if (operation == "preProcess") {
        if(controller->getGraph() == nullptr)
            throw AppException("You must read the graph firstly, before running this operation");

        if(!controller->getGraph()->preProcess(controller->getCentral())) throw AppException("Vertex does not exist");

        viewer = new GraphVisualizer(600, 600);
        viewer->draw(controller->getGraph());
    }

    else if (operation == "shortestPath") {
        string variant;
        int origin, dest;

        if (!(line >> variant)) throw AppException("Incorrect number of parameters");
        if (!(line >> origin) || !(line >> dest)) throw AppException("Incorrect number of parameters");

        if (variant == "dijkstra") controller->shortestPath(1, origin, dest);
        else if (variant == "dijkstraOriented") controller->shortestPath(2, origin, dest);
        else if (variant == "dijkstraBidirectional") controller->shortestPath(3, origin, dest);

        controller->showGraph();
    }

    else if(operation == "setCentral") {
        int centralId;
        if (!(line >> centralId)) throw AppException("Incorrect number of parameters");
        controller->setCentral(centralId);
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

#endif //MEAT_WAGONS_APPLICATION_H