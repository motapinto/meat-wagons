#include <iostream>
#include <sstream>
#include <string>
#include "Application/Application.h"

using namespace std;

int main(int argc, char* argv[]) {
    try {
        Application application = Application();
        while(true) if(!application.run()) break;
    } catch (MeatWagonsException &e) {
        cout << "Exception: " + e.getMessage() << endl;
        cout << "Usage: ./meat-wagons" << endl << endl;
    }
}