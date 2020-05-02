#include <iostream>
#include <sstream>
#include <string>
#include "Application/Application.h"

using namespace std;

int main(int argc, char* argv[]) {
    try {
        if(argc != 2) throw AppException("Wrong number of arguments");
        Application application = Application(argv[1]);
        while(true) application.run();
    } catch (AppException &e) {
        cout << "Exception: " + e.getMessage() << endl;
        cout << "Usage: ./meat-wagons <graph file path>" << endl << endl;
    }
}