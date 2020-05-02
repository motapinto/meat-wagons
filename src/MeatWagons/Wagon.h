#pragma once
#ifndef WAGON_H_
#define WAGON_H_

#include "Delivery.h"

class Wagon {
    private:
        int id, capacity;
        Time nextAvailableTime;
        vector<Delivery> deliveries;

    public:
        Wagon(const int id, const int capacity) : id(id), capacity(capacity) {}

        int getId() const;
        void setId(const int id);

        int  getCapacity() const;
        void setCapacity(const int capacity);

        Time getNextAvailableTime() const;
        void setNextAvailableTime(const Time &time);

        void addDelivery(const Delivery &delivery);
        bool operator<(Wagon &Wagon) const;
};

bool Wagon::operator<(Wagon &wagon) const {
    return this->capacity  < wagon.capacity;
}

int Wagon::getId() const {
    return this->id;
}

void Wagon::setId(const int id) {
    this->id = id;
}

int  Wagon::getCapacity() const {
    return this->capacity;
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

void Wagon::addDelivery(const Delivery &delivery) {
    this->deliveries.push_back(delivery);
}

#endif