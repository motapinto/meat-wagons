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
        Wagon(const int id, const int capacity) : id(id), capacity(capacity) {}

        int getId() const;
        void setId(const int id);

        int  getCapacity() const;
        void setCapacity(const int capacity);

        Time getNextAvailableTime() const;
        void setNextAvailableTime(const Time &time);

        const vector<Delivery*> getDeliveries() const;
        void setDeliveries(const vector<Delivery*> deliveries);
        void addDelivery(Delivery* delivery);

        int getSpaceLeft() const;

        void init();

        bool operator<(const Wagon &wagon) const;
};

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

const vector<Delivery*> Wagon::getDeliveries() const {
    return this->deliveries;
}

void Wagon::setDeliveries(const vector<Delivery*> deliveries) {
    this->deliveries = deliveries;
}

void Wagon::addDelivery(Delivery *delivery) {
    this->deliveries.push_back(delivery);
}

int Wagon::getSpaceLeft() const {
    return this->capacity - this->deliveries.size();
}

void Wagon::init() {
    this->deliveries.clear();
    this->nextAvailableTime = Time();
}

bool Wagon::operator<(const Wagon &wagon) const {
    return this->capacity < wagon.capacity;
}

#endif