#pragma once
#ifndef MEAT_WAGONS_DELIVERY_H
#define MEAT_WAGONS_DELIVERY_H

#include "Request.h"

class Delivery {
    private:
        vector<Request> requests;
        vector<Vertex*> path;
    public:
        Delivery(vector<Request> requests, vector<Vertex*> path) {
            this->requests = requests;
            this->path = path;
        }

        const vector<Request> getRequests() const;
        const vector<Vertex *> getPath() const;
        void setRequests(const vector<Request> &requests);
        void setPath(const vector<Vertex*> &path);
};

const vector<Request> Delivery::getRequests() const {
    return requests;
}

const vector<Vertex *> Delivery::getPath() const {
    return path;
}

void Delivery::setRequests(const vector<Request> &requests) {
    this->requests = requests;
}

void Delivery::setPath(const vector<Vertex*> &path) {
    this->path = path;
}

#endif //MEAT_WAGONS_DELIVERY_H