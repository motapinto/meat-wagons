#pragma once
#ifndef MEAT_WAGONS_APPLICATION_H
#define MEAT_WAGONS_APPLICATION_H

#include <fstream>
#include "../MeatWagons/MeatWagons.h"

class Application {
    private:
        MeatWagons *controller = new MeatWagons(10, 100);
        GraphVisualizer *viewer = nullptr;

    public:
        static void usage();
        bool run();
};

void Application::usage() {
    cout << "Menu Options:" << endl;
    cout << "\treadGraph <graph folder path>" << endl;
    cout << "\tpreProcess {<node id>}" << endl;
    cout << "\tshortestPath {dijkstra, dijkstraOriented, dijkstraBidirectional} <origin node> <destination node>" << endl;
    cout << "\tdeliver <iteration{1,2,3}>"<< endl;
    cout << "\tsetCentral <node id>"<< endl;
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
    }

    else if (operation == "preProcess") {
        int node;

        if (!(line >> node)) controller->preProcess(controller->getCentral());
        else controller->preProcess(node);
    }

    else if (operation == "shortestPath") {
        string variant;
        int origin, dest;

        if (line >> variant && line >> origin && line >> dest) {
            if (variant == "dijkstra") controller->shortestPath(1, origin, dest);
            else if (variant == "dijkstraOriented") controller->shortestPath(2, origin, dest);
            else if (variant == "dijkstraBidirectional") controller->shortestPath(3, origin, dest);
        }
    }

    else if(operation == "deliver") {
        int iteration;
        if(line >> iteration) controller->deliver(iteration);
    }

    else if(operation == "setCentral") {
        int centralId;
        if (line >> centralId) controller->setCentral(centralId);
    }

    else if(operation == "addRequest") {

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