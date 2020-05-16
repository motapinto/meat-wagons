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
        Time realArrival, realDeliver;
        int distFromCentral;

    public:
        Request() {}
        Request(const string &prisoner, const int dest, const int priority, const Time &arrival) :
            prisoner(prisoner), dest(dest), priority(priority), arrival(arrival) {}

        string getPrisoner() const;

        int getDest() const;

        int getPriority() const;

        Time getArrival() const;

        Time getRealArrival() const;
        void setRealArrival(const Time &time);

        Time getRealDeliver() const;
        void setRealDeliver(const Time &time);

        bool getAssigned() const;
        void setAssigned(const bool value);

        int getDistFromCentral() const;
        void setDistFromCentral(int distFromCentral);

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

bool Request::getAssigned() const {
    return this->assigned;
}

Time Request::getRealArrival() const {
    return this->realArrival;
}

Time Request::getRealDeliver() const {
    return this->realDeliver;
}

void Request::setRealArrival(const Time &time){
    this->realArrival = time;
}

void Request::setRealDeliver(const Time &time){
    this->realDeliver = time;
}

void Request::setAssigned(const bool value) {
    this->assigned = value;
}

bool Request::operator<(const Request &request) const {
    return this->arrival < request.arrival;
}

int Request::getDistFromCentral() const {
    return distFromCentral;
}

void Request::setDistFromCentral(int distFromCentral) {
    this->distFromCentral = distFromCentral;
}

#endif //MEAT_WAGONS_REQUEST_H