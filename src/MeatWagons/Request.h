#pragma once
#ifndef MEAT_WAGONS_REQUEST_H
#define MEAT_WAGONS_REQUEST_H

#include "Time.h"

class Request {
    private:
        string prisoner;
        int dest, priority;
        Time arrival, deliver;
        bool assigned;

    public:
        Request(string prisoner, int dest, int priority, Time arrival) {
            this->prisoner = prisoner;
            this->dest = dest;
            this->priority = priority;
            this->arrival = arrival;
            this->assigned = false;
        }

        const string getPrisoner() const;
        const int getDest() const;
        const int getPriority() const;
        const Time getArrival() const;
        const Time getDelivery() const;
        const bool getAssigned() const;
        const void setAssigned(bool assigned);
};

const string Request::getPrisoner() const {
    return this->prisoner;
}

const int Request::getDest() const {
    return this->dest;
}

const int Request::getPriority() const {
    return this->priority;
}

const Time Request::getArrival() const {
    return this->arrival;
}
const Time Request::getDelivery() const {
    return this->deliver;
}

const bool Request::getAssigned() const {
    return this->assigned;
}

const void Request::setAssigned(bool assigned) {
    this->assigned = assigned;
}

#endif //MEAT_WAGONS_REQUEST_H