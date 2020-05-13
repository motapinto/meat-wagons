#pragma once
#ifndef MEAT_WAGONS_APPLICATION_H
#define MEAT_WAGONS_APPLICATION_H
#include <sstream>
#include <fstream>
#include <iomanip>
#include "../utils.h"
#include "../MeatWagons/MeatWagons.h"

class Application {
    private:
        MeatWagons *controller = new MeatWagons(1, 100);
        GraphVisualizer *viewer = nullptr;

    public:
        void run();
        void displayMenu();
};

void Application::displayMenu()
{
    cout << "---------------------------------------------------------------------" << endl;
    cout << "Menu Options: [Type 'back' to go back in any menu]" << endl;
    cout << "1 - Read Graph" << endl;
    if(controller->getGraph() != nullptr) {cout << "2 - Pre Process" << endl;}
    if(controller->getGraph() != nullptr) {cout << "3 - Shortest Path" << endl;}
    if(controller->getGraph() != nullptr) {cout << "4 - Deliver" << endl;}
    if(controller->getGraph() != nullptr) {cout << "5 - Set Central" << endl;}
    if(controller->getGraph() != nullptr) {cout << "6 - Wagon Operation" << endl;}
    if(controller->getGraph() != nullptr) {cout << "7 - List Requests" << endl;}
    cout << "0 - Exit" << endl << endl;

    if(controller->getGraph() == nullptr) {cout << "Graph not read yet!" << endl << endl;}
    else {
        cout << "Graph read for: '" << controller->getGraphName() << "'" << endl;
        cout << "Central node ID: " << controller->getCentral() << endl;
    }

    cout << endl << "\bInput: > ";
}

void Application::run() 
{
    int option;
    string argument, input;

    displayMenu();
    while(true) {
        readline(input);
        if((stoint(input, option) == 0 && controller->getGraph() == nullptr && (option == 0 || option == 1))) break;
        if(stoint(input, option) == 0 && controller->getGraph() != nullptr && option >= 0 && option <= 7) break;
        else cout << "\bInput: > ";
    }

    // curly brackets are needed to initialize new variables in case scopes
    switch (option) {
        case 0: {
            cout << endl << "Exiting..." << endl;
            exit(0);
        }
        case 1: {
            cout << endl << "--- Read Graph ---";
            cout << endl << "Provide city name [Example: 'Porto']";
            cout << endl << "\bInput: > ";

            while(true) {
                readline(input);
                if(input == "back") break;
                else if (input == "Aveiro" || input == "Braga" || input == "Coimbra" || input == "Ermesinde" || input == "Fafe" || input == "Gondomar" || input == "Lisboa" || input == "Maia" || input == "Porto" || input == "Viseu" || input == "Portugal") {
                    controller->setGraph("maps/PortugalMaps/" + input);
                    break;
                }
                else cout << "\bInput: > ";
            }

            break;
        }
        case 2: {
            cout << endl << "--- Processing node ---";
            cout << endl << "Provide <node id> [Example: 'central']>";
            cout << endl << "\bInput: > ";

            int node;
            while(true) {
                readline(input);
                if(input == "back") break;
                else if(input == "central") {
                    controller->preProcess(controller->getCentral());
                    break;
                }
                else if(stoint(input, node) == 0 && controller->getGraph()->findVertex(node) != nullptr) {
                    controller->preProcess(node);
                    break;
                }
                else cout << "\bInput: > ";
            }

            break;
        }
        case 3: {
            cout << endl << "--- Shortest Path ---";
            cout << endl << "1 - Original Dijkstra";
            cout << endl << "2 - Oriented Dijkstra (A*)";
            cout << endl << "3 - Bidirectional Oriented Dijkstra(Oriented A*)";
            cout << endl << "Input: > ";

            while(true) {
                readline(input);
                if(input == "back") break;
                else if(stoint(input, option) == 0 && option >= 1 && option <= 3) {
                    int origin, dest;
                    while(true) {
                        cout << endl << "Provide <origin node> <destination node> [Example: 90379359 411018963]";
                        cout << endl << "\bInput: > ";

                        readline(input);
                        if(input == "back") break;
                        stringstream line(input);

                        if (line >> origin && line >> dest && controller->getGraph()->findVertex(origin) != nullptr && controller->getGraph()->findVertex(dest) != nullptr)
                            controller->shortestPath(option, origin, dest); break;
                    }
                }
                else cout << "\bInput: > ";
            }

            break;
        }
        case 4: {
            cout << endl << "--- Delivering ---" << endl;
            cout << "1 - Single Wagon with capacity 1" << endl;
            cout << "2 - Single Wagon that groups requests" << endl;
            cout << "3 - Multiple Wagons that groups requests" << endl;
            cout << "4 - Set Maximum Distance between Deliveries" << endl;
            cout << "Current ZoneMaxDist: " << controller->getMaxDist() << endl << ">";

            int choice;
            bool back = false;

            while(true) {
                readline(input);
                if(input == "back") {
                    back = true;
                    break;
                }
                else if(stoint(input, choice) != 0 || choice < 0 || choice > 4) {
                    cout << "\nTry again\n>";
                    break;
                }
                else break;
            }
            if(back) break;

            if(choice == 4) {
                int newMaxDist;
                cout << "\n--- Setting new max distance ---\n>";
                readline(input);
                while(stoint(input, newMaxDist) != 0 || newMaxDist < 1) readline(input);
                controller->setMaxDist(newMaxDist);
            }
            else {
                controller->deliver(choice);
                int wagon, delivery;
                while(true) {
                    //check wagon & delivery index
                    cout << "OLAAAAAAAAAAAAAAAAAAAAAAA" << endl;
                    cout << "\n--- Choose Wagon ---" << endl;
                    cout << "\tWagon Index: ";

                    readline(input);
                    if(input == "back") {
                        back = true;
                        break;
                    }
                    else if(stoint(input, wagon) != 0 || wagon < 0) {
                        cout << "\nTry again\n>";
                    }
                    else break;
                }
                if(back) break;

                while(true) {
                    cout << "\tDelivery Index: ";
                    readline(input);
                    if(stoint(input, delivery) != 0 || delivery < 0) {
                        cout << "\nTry again\n>";
                    }
                    else break;
                }

                Delivery *deliveryChosen = controller->drawDeliveries(wagon, delivery);
                cout << "Leaving at: " << deliveryChosen->getStart() << endl;
                cout << "Returns at: " << deliveryChosen->getEnd() << endl;
                cout << "Requests done:" << endl;
                for(auto r : deliveryChosen->getRequests()) {
                    cout << "\t" << "Prisoner: " << setfill(' ') << setw(10) << r.getPrisoner() << endl;
                    cout << "\t" << "Priority: " << r.getPriority() ;
                    cout << "\t" << "Arrives at: " << r.getArrival();
                    cout << "\t" << "Delivered at: " << r.getDelivery();
                }
            }
            break;
        }
        case 5: {
            cout << "\n--- Setting Central Node ---";
            cout << "\nCurrent Central Node ID: " << controller->getCentral();
            cout << "\nProvide <node id>\n>";

            int centralID;
            bool back = false;

            while(true) {
                readline(input);
                if(input == "back") {
                    back = true;
                    break;
                }
                else if(stoint(input, centralID)) {
                    cout << "\nTry again\n>";
                    break;
                }
                else break;
            }
            if(back) break;

            Vertex *v = controller->getGraph()->findVertex(centralID);
            
            if(v == nullptr) cout << "Vertex does not exist" << endl << endl;
            else controller->setCentral(centralID);

            break;
        }
        case 6: {
            cout << "\n--- Wagon Operation --- \t\t\t(Type 'back' to go back)\n";
            cout << "\t1 - List Wagons\n";
            cout << "\t2 - Add Wagons\n";
            cout << "\t3 - Remove Wagons\n>";

            while(true) {
                int choice;
                readline(input);
                if(input == "back") break;
                else if(stoint(input, choice) || choice < 1 || choice > 3) {
                    cout << "\nTry again\n>";
                    break;
                }

                if(choice == 1) {
                    cout << "\n--- Listing Wagons ---" << endl;
                    controller->listWagons();
                    break;
                }
                else if(choice == 2) {
                    cout << "\n--- Adding Wagons ---" << endl;
                    cout << "Provide <capacity>\n>";

                    readline(input);
                    int capacity;
                    stringstream line(input);
                    if (line >> capacity)
                        controller->addWagon(capacity);
                    break;
                }
                else if(choice == 3) {
                    cout << "\n--- Removing Wagons ---" << endl;
                    cout << "Provide <id> <capacity>\n>";

                    readline(input);
                    int id, capacity;
                    stringstream line(input);
                    if (line >> id && line >> capacity)
                        controller->removeWagon(id, capacity);
                    break;
                }
            }
            break;
        }
        case 7: {
            cout << "\n--- List Requests ---";
            controller->listRequests();
            break;
        }
        default:
            break;
    }
}

#endif //MEAT_WAGONS_APPLICATION_H
