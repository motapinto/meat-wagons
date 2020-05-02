#pragma once
#ifndef MEAT_WAGONS_REQUEST_H
#define MEAT_WAGONS_REQUEST_H

#include "Time.h"

using namespace std;

class Request {
    private:
        string prisoner;
        int dest, priority;
        Time arrival, deliver;
        bool assigned = false;

    public:
        Request(const string &prisoner, const int dest, const int priority, const Time &arrival) :
            prisoner(prisoner), dest(dest), priority(priority), arrival(arrival) {}

        string getPrisoner() const;
        int getDest() const;
        int getPriority() const;
        Time getArrival() const;
        Time getDelivery() const;
        bool getAssigned() const;
        void setAssigned(const bool assigned);

        bool operator<(const Request &request) const;
};

string Request::getPrisoner() const {
    return this->prisoner;
}

int Request::getDest() const {
    return this->dest;
}

int Request::getPriority() const {
    return this->priority;
}

Time Request::getArrival() const {
    return this->arrival;
}
Time Request::getDelivery() const {
    return this->deliver;
}

bool Request::getAssigned() const {
    return this->assigned;
}

void Request::setAssigned(const bool assigned) {
    this->assigned = assigned;
}

bool Request::operator<(const Request &request) const {
    return this->arrival < request.arrival;
}

#endif //MEAT_WAGONS_REQUEST_H
