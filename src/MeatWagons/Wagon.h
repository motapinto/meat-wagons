#pragma once
#ifndef WAGON_H_
#define WAGON_H_

#include "Delivery.h"

class Wagon {
    private:
        int id, capacity;
        Time nextAvailableTime;
        vector<Delivery*> deliveries;

    public:
        Wagon(int id, int capacity) : id(id), capacity(capacity) {}

        int getId() const;
        void setId(const int id);

        int  getCapacity() const;
        void setCapacity(const int capacity);

        Time getNextAvailableTime() const;
        void setNextAvailableTime(const Time &time) const;

        void addDelivery(Delivery *delivery);

    void setNextAvailableTime(const Time &nextAvailableTime);
};

int Wagon::getId() const {
    return id;
}

void Wagon::setId(const int id) {
    this->id = id;
}

int  Wagon::getCapacity() const {
    return capacity;
}

void Wagon::setCapacity(const int capacity) {
    this->capacity = capacity;
}

Time Wagon::getNextAvailableTime() const {
    return this->nextAvailableTime;
}

void Wagon::setNextAvailableTime(const Time &nextAvailableTime) {
    this->nextAvailableTime = nextAvailableTime;
}

void Wagon::addDelivery(Delivery *delivery) {
    this->deliveries.push_back(delivery);
}

#endif