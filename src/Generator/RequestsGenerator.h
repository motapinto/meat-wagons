/*#pragma once
#ifndef MEAT_WAGONS_REQUESTSGENERATOR_H
#define MEAT_WAGONS_REQUESTSGENERATOR_H

#include <Python.h>
// https://docs.python.org/3/extending/extending.html
// maybe later
class RequestsGenerator {
    private:
        string path;

    public:
        RequestsGenerator(string path) {
            this->path = path;
        }

        void generate();
};

void RequestsGenerator::generate() {
    Py_Initialize();
    PyRun_SimpleString();
    Py_Finalize();
    return;
}


#endif //MEAT_WAGONS_REQUESTSGENERATOR_H*/