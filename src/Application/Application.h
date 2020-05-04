#pragma once
#ifndef MEAT_WAGONS_APPLICATION_H
#define MEAT_WAGONS_APPLICATION_H
#include <fstream>
#include "../MeatWagons/MeatWagons.h"
#define divider "---------------------------------------------------"

#ifdef _WIN32
#define clearScreen() system("cls");
#else
#define clearScreen() system("cls");
#endif

void readline(string &str) {
    cin.clear();
    fflush(stdin);
    getline(cin, str);
    while (str.size() == 0)
    {
        getline(cin, str);
    }
}
int stoint(const string &str, int &value)
{
    int base = 10;
    size_t *pos = 0;
    // wrapping stoi because it may throw an exception

    try {
        value = stoi(str, pos, base);
        return 0;
    }
    catch (const invalid_argument &ia) {
        //cerr << "Invalid argument: " << ia.what() << endl;
        return -1;
    }

    catch (const out_of_range &oor) {
        //cerr << "Out of Range error: " << oor.what() << endl;
        return -2;
    }

    catch (const exception &e) {
        //cerr << "Undefined error: " << e.what() << endl;
        return -3;
    }
}
// ===========================================================

class Application {
    private:
        MeatWagons *controller = new MeatWagons(10, 100);
        GraphVisualizer *viewer = nullptr;

    public:
        void run();
        static void displayMenu();
};


void Application::displayMenu()
{
    cout << divider << endl;
    cout << "Menu Options:" << endl;
    cout << "\t1 - Read Graph <graph & requests folder path>" << endl;
    cout << "\t2 - Process <node id>" << endl;
    cout << "\t3 - Shortest Path <{dijkstra, dijkstraOriented, dijkstraBidirectional}> <origin node> <destination node>" << endl;
    cout << "\t4 - Deliver (Iteration: <{1,2,3}>)"<< endl;
    cout << "\t5 - Set Central <node id>"<< endl;
    cout << "\t6 - Wagon Operation <{list | add | remove}> <operands>"<< endl;
    cout << "\t7 - Requests Operation <{list | add | remove}> <operands>"<< endl;

    cout << "\t0 - Exit" << endl << endl;
    cout << "\b> ";
}

void Application::run() 
{
    int option;
    string argument, input;

    displayMenu();
    while(1) {
        readline(input);
        int h = stoint(input, option);
        if(h == 0) break;
        else cout << "> ";
    }

    switch (option)
    {
        // curly brackets are needed to initialize new variables in case scopes
        case 0: {
            cout << endl << "Exiting";
            exit(0);
        }
        case 1: {
            cout << "\n--- Reading Graph ---";
            cout << "\nProvide the <graph and requests folder path>";
            cout << "\nOr type 0 to go back";
            cout << "\n> ";

            readline(input); if(input == "0") break;
            stringstream line(input);
            controller->setGraph(line.str());

            break;
        }
        case 2: {
            cout << "\n--- Processing node ---";
            cout << "\nProvide the following <node id>";
            cout << "\nOr type 0 to go back";
            cout << "\n> ";

            int node;
            readline(input); if(input == "0") break;
            stringstream line(input);

            if (!(line >> node)) controller->preProcess(controller->getCentral());
            else controller->preProcess(node);
            break;
        }
        case 3: {
            cout << "\n--- Finding Shortest Path ---";
            cout << "\nProvide the following <{dijkstra, dijkstraOriented, dijkstraBidirectional}> <origin node> <destination node>";
            cout << "\nOr type 0 to go back";
            cout << "\n> ";

            string variant;
            int origin, dest;
            readline(input); if(input == "0") break;
            stringstream line(input);

            if (line >> variant && line >> origin && line >> dest) {
                if (variant == "dijkstra") controller->shortestPath(1, origin, dest);
                else if (variant == "dijkstraOriented") controller->shortestPath(2, origin, dest);
                else if (variant == "dijkstraBidirectional") controller->shortestPath(3, origin, dest);
            }
            break;
        }
        case 4: {
            cout << "\n--- Delivering ---";
            cout << "\nProvide the following <{1,2,3}>";
            cout << "\nOr type 0 to go back";
            cout << "\n> ";

            int iteration;
            readline(input); if(input == "0") break;
            stringstream line(input);
            // if(line >> iteration) controller->deliver(iteration);

            break;
        }
        case 5: {
            cout << "\n--- Setting Central Node ---";
            cout << "\nProvide the following <node id>";
            cout << "\nOr type 0 to go back";
            cout << "\n> ";

            int centralId;
            readline(input); if(input == "0") break;
            stringstream line(input);
            if (line >> centralId) controller->setCentral(centralId);

            break;
        }
        case 6: {
            cout << "\n--- Wagon Operation ---";
            cout << "\nProvide the following <{list | add | remove}> <operands>";
            cout << "\nOr type 0 to go back";
            cout << "\n> ";

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
            cout << "\nOr type 0 to go back";
            cout << "\n> ";

            readline(input); if(input == "0") break;
            stringstream line(input);
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

            break;
        }
        default:
            break;
    }
}

#endif //MEAT_WAGONS_APPLICATION_H
