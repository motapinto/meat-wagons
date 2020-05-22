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
        MeatWagons *controller = new MeatWagons(1);

    public:
        void run();
        void displayMenu();
};

void Application::displayMenu()
{
    cout << "\n---------------------------------------------------------------------" << endl;
    cout << "Menu Options: [Type 'back' to go back in any menu]" << endl;
    cout << "1 - Read Graph" << endl;
    if(controller->getGraph() != nullptr) {cout << "2 - Pre Process" << endl;}
    if(controller->getGraph() != nullptr) {cout << "3 - Shortest Path" << endl;}
    if(controller->getGraph() != nullptr) {cout << "4 - Deliver" << endl;}
    if(controller->getGraph() != nullptr) {cout << "5 - Set Central" << endl;}
    if(controller->getGraph() != nullptr) {cout << "6 - Wagon Operation" << endl;}
    if(controller->getGraph() != nullptr) {cout << "7 - List Requests" << endl;}
    cout << "0 - Exit" << endl << endl;

    if(controller->getGraph() == nullptr) cout << "Graph not read yet!" << endl;
    else {
        cout << "Graph read for: '" << controller->getGraphName() << "'" << endl;
        cout << "Central node ID: " << controller->getCentral() << endl;
    }

    cout << "---------------------------------------------------------------------" << endl;
    cout << "\bInput: > ";
}

void Application::run() 
{
    int option;
    string argument, input;

    displayMenu();
    while(true) {
        readline(input);
        if(stoint(input, option) == 0 && controller->getGraph() == nullptr && (option == 0 || option == 1)) break;
        if(stoint(input, option) == 0 && controller->getGraph() != nullptr && option >= 0 && option <= 7) break;
        else cout << "\bInput: > ";
    }

    // curly brackets are needed to initialize new variables in case scopes
    switch (option) {
        case 0: exit(0);
        case 1: {
            cout << endl << "--- Read Graph ---";
            cout << endl << "Provide city name [Example: 'Porto']";
            cout << endl << "\bInput: > ";

            int tries = 0;
            while(true) {
                readline(input);
                tries++;
                if(input == "back") break;
                else if (input == "Aveiro" || input == "Braga" || input == "Coimbra" || input == "Ermesinde" || input == "Fafe" || input == "Gondomar" || input == "Lisboa" || input == "Maia" || input == "Porto" || input == "Viseu" || input == "Portugal") {
                    controller->setGraph("maps/PortugalMaps/" + input);
                    break;
                }
                else if(input == "4x4" || input == "8x8" || input == "16x16") {
                    controller->setGraph("maps/GridGraphs/" + input);
                    break;
                }
                else if(tries < 3) cout << "\bInput: > ";
                else {
                    controller->setGraph("input");
                    break;
                }
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
                else if(stoint(input, node) == 0) {
                    if(!controller->preProcess(node)) cout << "\bInput: > ";
                    else break;
                }
                else cout << "\bInput: > ";
            }

            break;
        }
        case 3: {
            bool back;
            while(true) {
                cout << endl << "--- Shortest Path ---";
                cout << endl << "1 - Classic Dijkstra";
                cout << endl << "2 - Oriented Dijkstra (A*)";
                cout << endl << "3 - Bidirectional Dijkstra";
                cout << endl << "\bInput: > ";
                readline(input);

                if(input == "back") break;
                else if(stoint(input, option) == 0 && option >= 1 && option <= 3) {
                    int origin, dest;
                    cout << endl << "\nProvide <origin node> <destination node> [Example: 90379359 411018963]";
                    cout << endl << "\bInput: > ";
                    while(true) {
                        string secondInput;
                        readline(secondInput);
                        if(secondInput == "back") break;

                        stringstream line(secondInput);
                        if (line >> origin && line >> dest && controller->shortestPath(option, origin, dest)) {
                            back = true;
                            break;
                        }
                        else cout << "\bInput: > ";
                    }
                }
                else {
                    cout << "\bInput: > ";
                    continue;
                }
                if(back) break;
            }
            break;
        }
        case 4: {
            while(true) {
                cout << endl << "--- Delivering --- [Number of Wagons = " << controller->getWagons().size() << "]" ;
                cout << endl << "1 - Single Wagon with capacity 1 [Restrictions: 1 wagon with capacity 1]";
                cout << endl << "2 - Single Wagon that groups requests [Restrictions: 1 wagon with capacity > 1]";
                cout << endl << "3 - Multiple Wagons that groups requests [Restrictions: > 1 wagon]";
                cout << endl << "4 - Set Maximum Distance between Deliveries [Current ZoneMaxDist = " << controller->getMaxDist() << "]" << endl;

                cout << "\n--- Listing Wagons ---" << endl;
                multiset<Wagon> wagons = this->controller->getWagons();
                for(const auto & wagon : wagons) {
                    cout << "[Wagon " << wagon.getId() << "] with capacity " << wagon.getCapacity() << endl;
                }
                cout << endl;

                cout << endl << "\bInput: > ";
                readline(input);
                int choice;
                if(input == "back") break;
                else if(stoint(input, choice) == 0 && choice >= 0 && choice <= 4) {
                    if (choice != 4) {
                        if(!controller->deliver(choice)) { cout << "Wrong iteration configuration"  << endl; continue; }
                        int wagon;
                        while (true) {
                            cout << endl << "--- Choose Wagon ---" << endl;
                            cout << "Wagon Index > ";

                            string wagonInput;
                            readline(wagonInput);
                            if (wagonInput == "back") break;
                            else if (stoint(wagonInput, wagon) == 0 && wagon >= 0) {
                                int delivery;
                                while (true) {
                                    cout << endl << "--- Choose a Delivery done by Wagon[" << wagon << "] --- "<< endl;
                                    cout << "Delivery Index > ";

                                    string deliveryInput;
                                    readline(deliveryInput);
                                    if (deliveryInput == "back") break;
                                    else if (stoint(deliveryInput, delivery) == 0 && delivery >= 0) {
                                        Delivery *deliveryChosen = controller->drawDeliveriesFromThread(wagon, delivery);
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
                else continue;
            }

            break;
        }
        case 5: {
            cout << endl << "--- Change Central Node ---";
            cout << endl <<"Current Central Node ID: " << controller->getCentral();
            cout << endl << "Provide <node id>" << endl;

            int centralID;
            while(true) {
                cout << "\bInput: > ";
                readline(input);
                if(input == "back") break;
                else if(stoint(input, centralID) == 0 && controller->setCentral(centralID)) break;
                else continue;
            }

            break;
        }
        case 6: {
            while(true) {
                cout << endl << "--- Wagon Operation --- ";
                cout << endl << "1 - List Wagons";
                cout << endl << "2 - Add Wagons";
                cout << endl << "3 - Remove Wagons";
                cout << endl << "\bInput: > ";

                int choice;
                readline(input);
                if(input == "back") break;
                else if(stoint(input, choice) || choice < 1 || choice > 3) continue;

                if(choice == 1) {
                    cout << "\n--- Listing Wagons ---" << endl;
                    multiset<Wagon> wagons = this->controller->getWagons();
                    for(const auto & wagon : wagons) {
                        cout << "[Wagon " << wagon.getId() << "] Capacity " << wagon.getCapacity() << " available at "
                             << wagon.getNextAvailableTime() << endl;
                    }
                    cout << endl;
                }
                else if(choice == 2) {
                    while(true) {
                        cout << endl << "--- Adding a Wagon ---";
                        cout << endl << "Provide <capacity>";
                        cout << endl << "\bInput: > ";

                        string secondInput;
                        readline(secondInput);
                        if(secondInput == "back") break;
                        int capacity;
                        if(stoint(secondInput, capacity) == 0 && capacity > 0) controller->addWagon(capacity);
                        else continue;
                    }
                }
                else if(choice == 3) {
                    while(true) {
                        cout << endl << "--- Removing a Wagon ---" << endl;
                        cout << endl << "Provide <id> <capacity>" << endl;
                        cout << endl << "\bInput: > ";

                        string secondInput;
                        readline(secondInput);
                        if(secondInput == "back") break;
                        int id, capacity;
                        stringstream line(secondInput);
                        if (line >> id && line >> capacity) controller->removeWagon(id, capacity);
                        else continue;
                    }
                }
            }

            break;
        }
        case 7: {
            cout << "\n--- List Requests ---";
            multiset<Request*> requests = controller->getRequests();
            for (auto request : requests) {
                cout << "Prisoner " << request->getPrisoner() << " to be received in node " << request->getDest() << " with priority "
                     << request->getPriority() << " at " << request->getArrival() << endl;
            }

            break;
        }
        default:
            break;
    }
}

#endif //MEAT_WAGONS_APPLICATION_H
