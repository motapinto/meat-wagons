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
    cout << "\tread <graph & requests folder path>" << endl;
    cout << "\tprocess {<node id>}" << endl;
    cout << "\tshortestPath {dijkstra, dijkstraOriented, dijkstraBidirectional} <origin node> <destination node>" << endl;
    cout << "\tdeliver <iteration{1,2,3}>"<< endl;
    cout << "\tsetCentral <node id>"<< endl;
    cout << "\twagonOperation {list | add | remove} {operands}"<< endl;
    cout << "\trequestsOperation {list | add | remove} {operands}"<< endl;

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

    else if (operation == "read") {
        string fileName;

        if (!(line >> fileName)) controller->setGraph("maps/PortugalMaps/Porto");
        else controller->setGraph(fileName);
    }

    else if (operation == "process") {
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
        //if(line >> iteration) controller->deliver(iteration);
    }

    else if(operation == "setCentral") {
        int centralId;
        if (line >> centralId) controller->setCentral(centralId);
    }

    else if(operation == "wagonOperation") {
        string variant;
        if(line >> variant) {
            if(variant == "list") controller->listWagons();

            else if(variant == "add") {
                int capacity;
                if (line >> capacity) controller->addWagon(capacity);
            }

            else if(variant == "remove") {
                int id, capacity;
                if (line>> id && line >> capacity) controller->removeWagon(id, capacity);
            }
        }
    }

    else if(operation == "requestsOperation") {
        string variant;
        if(line >> variant) {
            if(variant == "list") controller->listRequests();

            else if(variant == "add") {
                string prisioner;
                int dest, priority, hour, minute, second;
                if (line >> prisioner >> dest >> priority >> hour >> minute >> second)
                    controller->addRequest(prisioner, dest, priority, Time(hour, minute, second));
            }

            else if(variant == "remove") {
                string prisioner;
                int dest, priority, hour, minute, second;
                if (line >> prisioner >> dest >> priority >> hour >> minute >> second)
                    controller->removeRequest(prisioner, dest, priority, Time(hour, minute, second));
            }
        }
    }

    return true;
}

#endif //MEAT_WAGONS_APPLICATION_H