#pragma once
#ifndef MEAT_WAGONS_REQUEST_H
#define MEAT_WAGONS_REQUEST_H

class Request {
    private:
        string prisioner;
        int dest, priority;
        bool assigned;

    public:
        Request(string prisioner, int dest, int priority) {
            this->prisioner = prisioner;
            this->dest = dest;
            this->prisioner = priority;
            this->assigned = false;
        }

        const string getPrisioner() const;
        const int getDest() const;
        const int getPriority() const;
        const bool getAssigned() const;
        void setAssigned(bool assigned);
};

const string Request::getPrisioner() const {
    return this->prisioner;
}

const int Request::getDest() const {
    return this->dest;
}

const int Request::getPriority() const {
    return this->priority;
}

const bool Request::getAssigned() const {
    return this->assigned;
}

void Request::setAssigned(bool assigned) {
    this->assigned = assigned;
}

#endif //MEAT_WAGONS_REQUEST_H