#pragma once
#ifndef MEAT_WAGONS_APPLICATION_H
#define MEAT_WAGONS_APPLICATION_H
#include <sstream>
#include <fstream>
#include "../utils.h"
#include "../MeatWagons/MeatWagons.h"

class Application {
    private:
        MeatWagons *controller = new MeatWagons(1, 100);
        GraphVisualizer *viewer = nullptr;

    public:
        void run();
        void displayMenu();
        void calculateCentralVertex() const;
};


void Application::calculateCentralVertex() const {
    cout << endl;
    int i = 0, vertex = 0, max = 0, curr = 0;
    Graph aux = *controller->getGraph();
    for(auto vert : controller->getGraph()->getVertexSet()) {
        aux.preProcess(vert->getId());
        curr = aux.getVertexSet().size();
        if(curr > max) {
            max = curr;
            vertex = vert->getId();
        }
        aux = *controller->getGraph();
        i++;
        cout << i << "\tMAX: " << max << " CURR: " << curr << " V: " << vert->getId() << endl;
    }
    cout << "\nCentral Vertex ID: " << vertex << endl;    
}


void Application::displayMenu()
{
    cout << "---------------------------------------------------------------------" << endl;
    cout << "Menu Options: " << endl;
    cout << "\t1 - Read Graph" << endl;
    cout << "\t2 - Pre Process" << endl;
    cout << "\t3 - Shortest Path" << endl;
    cout << "\t4 - Deliver"<< endl;
    cout << "\t5 - Set Central"<< endl;
    cout << "\t6 - Wagon Operation <{list | add | remove}> <operands>"<< endl;
    cout << "\t7 - Requests Operation <{list | add | remove}> <operands>"<< endl;
    cout << "\t0 - Exit" << endl << endl;
        
    if(controller->getGraph() == nullptr) cout << "Graph not read yet!" << endl;
    else cout << "Graph read for: '" << controller->getGraphName() << "'" << endl;

    cout << ">";
}


void Application::run() 
{
    int option;
    string argument, input;

    displayMenu();
    while(true) {
        readline(input);
        if(stoint(input, option) == 0 && option >= 0 && option <= 7) 
            break;
        else cout << ">";
    }

    switch (option) {
        // curly brackets are needed to initialize new variables in case scopes
        case 0: {
            cout << endl << "Exiting";
            exit(0);
        }
        case 1: {
            cout << "\n--- Reading Graph --- (Type '0' or 'back' to go back)";
            cout << "\nProvide <graph folder path>";
            cout << "\nExample: 'maps/PortugalMaps/Porto' (or just 'Porto')\n>";

            bool back = false;
            readline(input);

            while(true) {
                if(input == "0" || input == "back") {
                    back = true;
                    break;
                }
                else if (input == "Aveiro" || input == "Braga" || input == "Coimbra" || input == "Ermesinde" || input == "Fafe" || input == "Gondomar" || input == "Lisboa" || input == "Maia" || input == "Porto" || input == "Viseu" || input == "Portugal") {
                    input = "maps/PortugalMaps/" + input;
                    break;
                }
                else if(strstr(strdup(input.c_str()), "maps/") == nullptr) {
                    cout << "\nTry again\n>";
                    readline(input);
                }
                else break;
            }
            if(back) break;

            stringstream line(input);
            string name = line.str();
            controller->setGraph(name);
            controller->setGraphName(name.substr(name.find_last_of('/') + 1));

            break;
        }
        case 2: {
            if(controller->getGraph() == nullptr) {
                cout << "No graph has been read yet!\n";
                break;
            }
            cout << "\n--- Processing node --- (Type '0' or 'back' to go back)";
            cout << "\nProvide <node id>\n>";

            int node;
            bool back = false;

            while(true) {
                readline(input);
                if(input == "0" || input == "back") {
                    back = true;
                    break;
                }
                else if(input == "central" || input == "c") {
                    node = controller->getCentral();
                    break;
                }
                else if(stoint(input, node) != 0 || node < 0) {
                    cout << "\nTry again\n>";
                }
                else break;
            }
            if(back) break;
            
            this->calculateCentralVertex();     
            // controller->preProcess(node);
            break;
        }
        case 3: {
            cout << "\n--- Finding Shortest Path --- (Type '0' or 'back' to go back)";
            cout << "\nProvide <Normal|Oriented|Bidirectional> <origin node> <destination node>";
            cout << "\nExample: 'Normal 90379359 411018963'\n>";

            string variant;
            int origin, dest;

            while(true) {
                readline(input);
                if(input == "0" || input == "back") break;

                stringstream line(input);
                if (line >> variant && line >> origin && line >> dest) 
                {
                    if (variant == "Normal" || variant == "N") {
                        controller->shortestPath(1, origin, dest);
                        break;
                    }
                    else if (variant == "Oriented" || variant == "O") {
                        controller->shortestPath(2, origin, dest);
                        break;
                    }
                    else if (variant == "Bidirectional" || variant == "B") {
                        controller->shortestPath(3, origin, dest);
                        break;
                    }
                    else cout << "\nTry again\n>";
                }
                else cout << "\nTry again\n>";
            }

            break;
        }
        case 4: {
            cout << "\n--- Delivering --- (Type '0' or 'back' to go back)\n";
            cout << "\t1 - Single Wagon with capacity 1" << endl; // dentro desta opcao: setMaxDist
            cout << "\t2 - Single Wagon that groups requests" << endl; // dentro dest opcao: setmaxdist setvans
            cout << "\t3 - Multiple Wagons that groups requests" << endl; // dentro dest opcao: setmaxdist setvans

            int iteration;
            bool back = false;

            while(true) {
                readline(input);
                if(input == "0" || input == "back") {
                    back = true;
                    break;
                }
                else if(stoint(input, iteration) != 0 || iteration < 0 || iteration > 3) {
                    cout << "\nTry again\n>";
                    break;
                }
                else break;
            }
            if(back) break;
            controller->deliver(iteration);
            break;
        }
        case 5: {
            if(controller->getGraph() == nullptr) {
                cout << "No graph has been read yet!\n";
                break;
            }
            cout << "\n--- Setting Central Node --- (Type '0' or 'back' to go back)";
            cout << "\nProvide <node id>\n>";

            int centralID;
            bool back = false;

            while(true) {
                readline(input);
                if(input == "0" || input == "back") {
                    back = true;
                    break;
                }
                else if(stoint(input, centralID)) {
                    cout << "\nTry again\n>";
                    break;
                }
            }
            if(back) break;

            stringstream line(input);
            if (line >> centralID) controller->setCentral(centralID);

            break;
        }
        case 6: {
            cout << "\n--- Wagon Operation --- (Type '0' or 'back' to go back)";
            cout << "\nProvide <list|add|remove> <operands>\n>";

            readline(input); if(input == "0") break;
            stringstream line(input);
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
            break;
        }
        case 7: {
            cout << "\n--- Requests ---";
            cout << "\nProvide the following <{1,2,3}>";
            cout << "\n(Type '0' or 'back' to go back)\n>";
            cout << "\n>";

            readline(input); if(input == "0") break;
            stringstream line(input);
            string variant;
            
            if(line >> variant) {
                if(variant == "list") controller->listRequests();
                else if(variant == "add") {
                    string prisoner;
                    int dest, priority, hour, minute, second;
                    if (line >> prisoner >> dest >> priority >> hour >> minute >> second)
                        controller->addRequest(prisoner, dest, priority, Time(hour, minute, second));
                }
                else if(variant == "remove") {
                    string prisoner;
                    int dest, priority, hour, minute, second;
                    if (line >> prisoner >> dest >> priority >> hour >> minute >> second)
                        controller->removeRequest(prisoner, dest, priority, Time(hour, minute, second));
                }
            }
            break;
        }
        default:
            break;
    }
}

#endif //MEAT_WAGONS_APPLICATION_H
