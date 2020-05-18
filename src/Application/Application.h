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
        MeatWagons *controller = new MeatWagons(3);

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
            cout << endl << "\bInput: > ";

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
            cout << endl << "--- Delivering ---";
            cout << endl << "1 - Single Wagon with capacity 1";
            cout << endl << "2 - Single Wagon that groups requests";
            cout << endl << "3 - Multiple Wagons that groups requests";
            cout << endl << "4 - Set Maximum Distance between Deliveries";
            cout << endl << "Current ZoneMaxDist: " << controller->getMaxDist();
            cout << endl << "\bInput: > ";

            int choice;
            while(true) {
                readline(input);
                if(input == "back") break;
                else if(stoint(input, choice) == 0 && choice >= 0 && choice <= 4) {
                    if (choice != 4) {
                        controller->deliver(choice);
                        int wagon, delivery;
                        while (true) {
                            cout << endl << "--- Choose Wagon ---" << endl;
                            cout << "Wagon Index > ";
                            readline(input);
                            if (input == "back") break;
                            else if (stoint(input, wagon) == 0 && wagon >= 0) {
                                while (true) {
                                    cout << endl << "--- Choose a Delivery from Wagon[" << wagon << "] --- "<< endl;
                                    cout << "Delivery Index > ";
                                    readline(input);
                                    cout << endl;
                                    if (input == "back") break;
                                    else if (stoint(input, delivery) == 0 && delivery >= 0) {
                                        Delivery *deliveryChosen = controller->drawDeliveries(wagon, delivery);
                                        cout << "Wagon[" << wagon << "] leaves central at: " << deliveryChosen->getStart() << endl;
                                        cout << "Wagon[" << wagon << "] returns to central at: " << deliveryChosen->getEnd() << endl;
                                        cout << "Requests done:" << endl;
                                        for (const Request *r : deliveryChosen->getRequests()) {
                                            cout << "\tPrisoner: " << setfill(' ') << setw(10) << r->getPrisoner();
                                            cout << "\tPriority: " << r->getPriority();
                                            cout << "\tArrives at: " << r->getRealArrival();
                                            cout << "\tDelivered at: " << r->getRealDeliver() << endl;
                                        }
                                    }
                                    else cout << endl << "\bInput: > ";
                                }
                            } else cout << endl << "\bInput: > ";
                        }
                    } else {
                        int newMaxDist;
                        cout << "\n--- Setting new max distance ---\n>";
                        readline(input);
                        while (true) {
                            readline(input);
                            if (input == "back") break;
                            else if (stoint(input, newMaxDist) == 0 || newMaxDist >= 1)
                                controller->setMaxDist(newMaxDist);
                            else cout << endl << "\bInput: > ";
                            readline(input);
                        }
                        break;
                    }
                }
                else {
                    cout << "\bInput: > ";
                    continue;
                }

                break;
            }

            break;
        }
        case 5: {
            cout << endl << "--- Change Central Node ---";
            cout << endl <<"Current Central Node ID: " << controller->getCentral();
            cout << endl << "Provide <node id>>";
            cout << endl << "\bInput: > ";

            int centralID;
            while(true) {
                readline(input);
                if(input == "back") break;
                else if(stoint(input, centralID) == 0) {
                    Vertex *v = controller->getGraph()->findVertex(centralID);
                    if(v == nullptr) cout << "Vertex does not exist" << endl << endl;
                    else controller->setCentral(centralID);

                    break;
                }
                else {
                    cout << "\bInput: > ";
                    continue;
                }

                break;
            }

            break;
        }
        case 6: {
            cout << endl << "--- Wagon Operation --- ";
            cout << endl << "1 - List Wagons";
            cout << endl << "2 - Add Wagons";
            cout << endl << "3 - Remove Wagons>";
            cout << endl << "\bInput: > ";

            while(true) {
                int choice;
                readline(input);
                if(input == "back") break;
                else if(stoint(input, choice) || choice < 1 || choice > 3) {
                    cout << endl << "\bInput: > ";
                    continue;
                }

                if(choice == 1) {
                    cout << "\n--- Listing Wagons ---" << endl;
                    controller->listWagons();
                    break;
                }
                else if(choice == 2) {
                    cout << endl << "--- Adding Wagons ---";
                    cout << endl << "Provide <capacity>";
                    cout << endl << "\bInput: > ";

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
