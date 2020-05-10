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
    cout << "Menu Options: " << endl;
    cout << "\t1 - Read Graph" << endl;
    cout << "\t2 - Pre Process" << endl;
    cout << "\t3 - Shortest Path" << endl;
    cout << "\t4 - Deliver"<< endl;
    cout << "\t5 - Set Central"<< endl;
    cout << "\t6 - Wagon Operation"<< endl;
    cout << "\t7 - Requests Operation"<< endl;
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
        if(stoint(input, option) == 0 && option >= 0 && option <= 7) break;
        else cout << ">";
    }

    // curly brackets are needed to initialize new variables in case scopes
    switch (option) {
        case 0: {
            cout << endl << "Exiting";
            exit(0);
        }
        case 1: {
            cout << "\n--- Reading Graph --- \t\t\t(Type '0' or 'back' to go back)";
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
                else if(strstr(_strdup(input.c_str()), "maps/") == nullptr) {
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
            cout << "\n--- Processing node --- \t\t\t(Type '0' or 'back' to go back)";
            cout << "\nProvide <node id> \t\t\t\t('central' for central node id)\n>";

            int node;
            bool back = false;

            while(true) {
                readline(input);
                if(input == "0" || input == "back") {
                    back = true;
                    break;
                }
                else if(input == "central") {
                    node = controller->getCentral();
                    break;
                }
                else if(stoint(input, node) != 0 || node < 0) {
                    cout << "\nTry again\n>";
                }
                else break;
            }
            if(back) break;

            if(controller->getGraph()->findVertex(node) == nullptr) {
                cout << "Vertex does not exist" << endl << endl;
                break;
            }
            
            controller->preProcess(node);
            break;
        }
        case 3: {
            cout << "\n--- Finding Shortest Path --- \t\t\t(Type '0' or 'back' to go back)";
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
            cout << "\n--- Delivering --- \t\t\t(Type '0' or 'back' to go back)\n";
            cout << "\t1 - Single Wagon with capacity 1" << endl;
            cout << "\t2 - Single Wagon that groups requests" << endl;
            cout << "\t3 - Multiple Wagons that groups requests" << endl;
            cout << "\t4 - Set Maximum Distance between Deliveries" << endl;
            cout << "Current ZoneMaxDist: " << controller->getMaxDist() << endl << ">";

            int choice;
            bool back = false;

            while(true) {
                readline(input);
                if(input == "0" || input == "back") {
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
                    cout << "\n--- Choose Wagon --- \t\t\t(Type '0' or 'back' to go back)" << endl;
                    cout << "\tWagon Index: ";

                    readline(input);
                    if(input == "0" || input == "back") {
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
                cout << "Leaving at: " << deliveryChosen->getStart();
                cout << "Returns at: " << deliveryChosen->getEnd();
                cout << "Requests done:" << endl;
                for(auto r : deliveryChosen->getRequests()) {
                    cout << "\t" << "Prisoner: " << setfill(' ') << setw(10) << r->getPrisoner() << endl;
                    cout << "\t" << "Priority: " << r->getPriority() ;
                    cout << "\t" << "Arrives at: " << r->getArrival();
                    cout << "\t" << "Delivered at: " << r->getDelivery();
                }
            }
            break;
        }
        case 5: {
            if(controller->getGraph() == nullptr) {
                cout << "No graph has been read yet!\n";
                break;
            }
            cout << "\n--- Setting Central Node --- \t\t\t(Type '0' or 'back' to go back)";
            cout << "\nCurrent Central Node ID: " << controller->getCentral();
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
                else break;
            }
            if(back) break;

            Vertex *v = controller->getGraph()->findVertex(centralID);
            
            if(v == nullptr) cout << "Vertex does not exist" << endl << endl;
            else controller->setCentral(centralID);

            break;
        }
        case 6: {
            if(controller->getGraph() == nullptr) {
                cout << "No graph has been read yet!\n";
                break;
            }
            cout << "\n--- Wagon Operation --- \t\t\t(Type '0' or 'back' to go back)\n";
            cout << "\t1 - List Wagons\n";
            cout << "\t2 - Add Wagons\n";
            cout << "\t3 - Remove Wagons\n>";

            while(true) {
                int choice;
                readline(input);
                if(input == "0" || input == "back") break;
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
            if(controller->getGraph() == nullptr) {
                cout << "No graph has been read yet!\n";
                break;
            }
            cout << "\n--- Requests --- \t\t\t(Type '0' or 'back' to go back)";
            cout << "\t1 - List Resquests\n";
            cout << "\t2 - Add Requests\n";
            cout << "\t3 - Remove Requests\n>";

            while (true) {
                int choice;
                readline(input);
                if (input == "0" || input == "back") break;
                else if (stoint(input, choice) || choice < 1 || choice > 3) {
                    cout << "\nTry again\n>";
                    break;
                }

                if(choice == 1) {
                    cout << "\n--- Listing Requests ---" << endl;
                    controller->listRequests();
                    break;
                }
                if(choice == 2) {
                    cout << "\n--- Adding Requests ---" << endl;
                    cout << "Provide <prisoner> <destination> <priority> <hour> <minutes> <seconds>\n>";

                    readline(input);
                    string prisoner;
                    stringstream line(input);
                    int dest, priority, hour, minute, second;
                    if (line >> prisoner >> dest >> priority >> hour >> minute >> second)
                        controller->addRequest(prisoner, dest, priority, Time(hour, minute, second));
                }
                if (choice == 3) {
                    cout << "\n--- Removing Requests ---" << endl;
                    cout << "Provide <prisoner> <destination> <priority> <hour> <minutes> <seconds>\n>";

                    readline(input);
                    string prisoner;
                    stringstream line(input);
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
