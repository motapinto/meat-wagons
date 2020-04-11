#pragma once
#ifndef POSITION_H_
#define POSITION_H_

#include <cmath>

class Position {
    private:
        double x, y;

    public:
        Position() {
            this->x = 0;
            this->y = 0;
        }

        Position(double x, double y) {
            this->x = x;
            this->y = y;
        }

        double getX() const {
            return this->x;
        }

         void setX(int x) const {
            this->x = x;
        }

         double getY() const {
            return this->y;
        }

         void setY() const {
            this->y = y;
        }

        double euclidianDistance(const Position &pos2) {
            return sqrt(pow(pos2.x - x, 2) + pow(pos2.y - y, 2));
        }
}

#endif