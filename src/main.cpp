#include <iostream>
#include <sstream>
#include <string>
#include "Application/Application.h"

using namespace std;

int main(int argc, char* argv[]) {
    try {
        Application application = Application();
        while(true) application.run();
    } catch (AppException &e) {
        cout << "Exception: " + e.getMessage() << endl;
        cout << "Usage: ./meat-wagons <graph file path>" << endl << endl;
    }
}