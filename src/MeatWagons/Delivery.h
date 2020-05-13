#pragma once
#ifndef MEAT_WAGONS_DELIVERY_H
#define MEAT_WAGONS_DELIVERY_H

class Delivery {
    private:
        Time start, end;
        vector<Request*> requests;
        vector<int> forwardPath;

    public:
        Delivery(const Time &start, const vector<Request*> &requests, const vector<int> &forwardPath, const int weight) {
            this->start = start;
            this->requests = requests;
            this->forwardPath = forwardPath;
            this->start.setSeconds(this->start.getSecond() + weight);
        }

        Time getStart() const;
        void setStart(const Time &start);

        Time getEnd() const;
        void setEnd(const Time &anEnd);

        vector<Request*> getRequests() const;
        void setRequests(const vector<Request*> &requests);
        void addRequest(Request *request);

        vector<int> getForwardPath() const;
        void setForwardPath(const vector<int> &path);

        vector<int> getBackwardPath() const;
        void setBackwardPath(const vector<int> &path);
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

void Delivery::setEnd(const Time &anEnd) {
    this->end = anEnd;
}

vector<Request*> Delivery::getRequests() const {
    return this->requests;
}

void Delivery::setRequests(const vector<Request*> &requests) {
    this->requests = requests;
}

void Delivery::addRequest(Request *request) {
    this->requests.push_back(request);
}

vector<int> Delivery::getForwardPath() const {
    return this->forwardPath;
}

void Delivery::setForwardPath(const vector<int> &path) {
    this->forwardPath = path;
}

#endif //MEAT_WAGONS_DELIVERY_H