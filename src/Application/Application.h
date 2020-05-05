#pragma once
#ifndef MEAT_WAGONS_APPLICATION_H
#define MEAT_WAGONS_APPLICATION_H
#include <sstream>
#include <fstream>
#include "../MeatWagons/MeatWagons.h"

void readline(string &str) {
    str.clear();
    cin.clear();
    fflush(stdin);
    getline(cin, str);
    while (str.empty()) getline(cin, str);
}
int stoint(const string &str, int &value) {
  // wrapping stoi because it may throw an exception
  try {
    value = stoi(str, nullptr, 10);
    return 0;
  }
  catch (const invalid_argument &ia) { return -1; }
  catch (const out_of_range &oor) { return -2; }
  catch (const exception &e) { return -3; }
}

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
    cout << "\b---------------------------------------------------" << endl;
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
    while(true) {
        readline(input);
        if(stoint(input, option) == 0 && option >= 0 && option <= 7) 
            break;
        else cout << "\b> ";
    }

    switch (option)
    {
        // curly brackets are needed to initialize new variables in case scopes
        case 0:
          cout << endl << "Exiting";
          exit(0);

        case 1: {
            cout << "\n--- Reading Graph ---";
            cout << "\nProvide a <graph & requests folder path> (type '0' or 'back' to go back)";
            cout << "\nExample: 'maps/PortugalMaps/Porto'\n> ";

            bool back = false;
            readline(input);

            while(true) {
                if(input == "0" || input == "back") {
                    back = true;
                    break;
                }
                else if(strstr(strdup(input.c_str()), "maps/") == nullptr) {
                    cout << "\nTry again\n> ";
                    readline(input);
                }
                else break;
            }
            if(back) break;
            
            stringstream line(input);
            cout << "|" << line.str() << "|\n";
            controller->setGraph(line.str());

            break;
        }

        case 2: {
            cout << "\n--- Processing node ---";
            cout << "\nProvide <node id>\n(Type '0' or 'back' to go back)\n> ";

            int node;
            bool back = false;
            readline(input);

            while(true) {
                if(input == "0" || input == "back") {
                    back = true;
                    break;
                }
                else if(stoint(input, node) != 0 || node < 0) {
                    cout << "\nTry again\n> ";
                    readline(input);
                }
                else break;
            }
            if(back) break;

            stringstream line(input);
            if (!(line >> node)) controller->preProcess(controller->getCentral());
            else controller->preProcess(node);

            break;
        }

        case 3: {
            cout << "\n--- Finding Shortest Path ---";
            cout << "\nProvide the following specs <{Normal, Oriented, Bidirectional}> <origin node> <destination node>";
            cout << "\n(Type '0' or 'back' to go back)\n> ";

            string variant;
            int origin, dest;
            bool back = false;
            readline(input);

            while(true) {
                if(input == "0" || input == "back") {
                    back = true;
                    break;
                }

                stringstream line(input);
                if (line >> variant && line >> origin && line >> dest) {
                    if (variant == "Normal" || variant == "N") controller->shortestPath(1, origin, dest);
                    else if (variant == "Oriented" || variant == "O") controller->shortestPath(2, origin, dest);
                    else if (variant == "Bidirectional" || variant == "B") controller->shortestPath(3, origin, dest);
                    else {
                        cout << "\nTry again\n> ";
                        readline(input);
                    }                    
                }
                else {
                    cout << "\nTry again\n> ";
                    readline(input);
                }
            }

            break;
        }

        case 4: {
            cout << "\n--- Delivering ---";
            cout << "\nProvide one of the following <{1,2,3}>";
            cout << "\n(Type '0' or 'back' to go back)\n> ";

            // int iteration;
            readline(input);
            if(input == "0" || input == "back") break;

            stringstream line(input);
            // if(line >> iteration) controller->deliver(iteration);

            break;
        }

        case 5: {
            cout << "\n--- Setting Central Node ---";
            cout << "\nProvide the following <node id>";
            cout << "\nType '0' or 'back' to go back";
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
            cout << "\nType 0 to go back";
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
            cout << "\nType 0 to go back";
            cout << "\n> ";

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
