#include "Application/Application.h"

int main(int argc, char* argv[]) {
    try {
        Application application = new Application(argc, argv);
        application.run();
    } catch (string exception) {
        cout << exception << endl;
        cout << "See usage below" << endl << endl;
        Application.usage();
    }
}