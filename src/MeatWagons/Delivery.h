#ifndef MEAT_WAGONS_DELIVERY_H
#define MEAT_WAGONS_DELIVERY_H


class Delivery {
    private:
        vector<Request> requests;
        vector<Vector*> path;
    public:
        Delivery(vector<Request> requests, vector<Vector*> path) {
            this->requests = requests;
            this->path = path;
        }
};


#endif //MEAT_WAGONS_DELIVERY_H
