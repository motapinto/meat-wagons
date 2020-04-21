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

        string getPrisioner();
        string getDest();
        string getPriority();
        string getAssigned();
        string setAssigned(bool assigned);
    };

string Request::getPrisioner() const {
    return this->prisioner;
}

string Request::getDest() const {
    return this->dest;
}

string Request::getPriority() const {
    return this->priority;
}

string Request::getAssigned() const {
    return this->assigned;
}

string Request::setAssigned(bool assigned) {
    this->assigned = assigned;
}


#endif //MEAT_WAGONS_REQUEST_H
