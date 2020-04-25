#pragma once
#ifndef MEAT_WAGONS_DELIVERY_H
#define MEAT_WAGONS_DELIVERY_H

class Delivery {
    private:
        Time start, end;
        vector<Request> requests;
        vector<Vertex*> path;

    public:
        Delivery(const vector<Request> &requests, const vector<Vertex*> path) : requests(requests), path(path) {}

        Time getStart() const;
        void setStart(const Time &start);

        Time getEnd() const;
        void setEnd(const Time &anEnd);

        vector<Request> getRequests() const;
        void setRequests(const vector<Request> &requests);
        void addRequest(const Request &request);

        vector<Vertex *> getPath() const;
        void setPath(const vector<Vertex*> &path);
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

vector<Request> Delivery::getRequests() const {
    return this->requests;
}

void Delivery::setRequests(const vector<Request> &requests) {
    this->requests = requests;
}

void Delivery::addRequest(const Request &request) {
    this->requests.push_back(request);
}

vector<Vertex *> Delivery::getPath() const {
    return this->path;
}

void Delivery::setPath(const vector<Vertex*> &path) {
    this->path = path;
}

#endif //MEAT_WAGONS_DELIVERY_H