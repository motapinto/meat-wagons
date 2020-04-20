#include "Application/Application.h"

int main(int argc, char* argv[]) {
    try {
        Application application = Application(argc, argv);
        application.run();
    } catch (AppException &e) {
        cout << e.getMessage() << endl;
        cout << "See usage below" << endl << endl;
        Application::usage();
    }
}