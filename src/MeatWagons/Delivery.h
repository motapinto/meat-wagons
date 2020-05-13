#pragma once
#ifndef MEAT_WAGONS_DELIVERY_H
#define MEAT_WAGONS_DELIVERY_H

class Delivery {
    private:
        Time start, end;
        vector<Request> requests;
        vector<int> forwardPath;
        int dropOff;

    public:
        Delivery(const Time &start, const vector<Request> &requests, const vector<int> &forwardPath, const int weight, const int dropOff) {
            this->start = start;
            this->requests = requests;
            this->forwardPath = forwardPath;
            this->end = start + Time(0, 0, this->start.toSeconds() + weight);
            this->dropOff = dropOff;
        }

        Time getStart() const;
        void setStart(const Time &start);

        Time getEnd() const;
        void setEnd(const Time &end);

        vector<Request> getRequests() const;
        void setRequests(const vector<Request> &requests);
        void addRequest(Request request);

        vector<int> getForwardPath() const;
        void setForwardPath(const vector<int> &path);

        int getDropOff() const;
        void setDropOff(int dropOff);
};

Time Delivery::getStart() const {
    return this->start;
}

void Delivery::setStart(const Time &start) {
    this->start = start;
}

Time Delivery::getEnd() const {
    return this->end;
}

void Delivery::setEnd(const Time &end) {
    this->end = end;
}

vector<Request> Delivery::getRequests() const {
    return this->requests;
}

void Delivery::setRequests(const vector<Request> &requests) {
    this->requests = requests;
}

void Delivery::addRequest(Request request) {
    this->requests.push_back(request);
}

vector<int> Delivery::getForwardPath() const {
    return this->forwardPath;
}

void Delivery::setForwardPath(const vector<int> &path) {
    this->forwardPath = path;
}

int Delivery::getDropOff() const {
    return dropOff;
}

void Delivery::setDropOff(const int dropOff) {
    this->dropOff = dropOff;
}

#endif //MEAT_WAGONS_DELIVERY_H