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

        Position(const double &x, const double &y) {
            this->x = x;
            this->y = y;
        }

        [[nodiscard]] double getX() const {
            return this->x;
        }

         void setX(double x) {
            this->x = x;
        }

         [[nodiscard]] double getY() const {
            return this->y;
        }

         void setY(double y) {
            this->y = y;
        }

        [[nodiscard]] double euclideanDistance(const Position &pos2) const {
            return sqrt(pow(pos2.x - x, 2) + pow(pos2.y - y, 2));
        }
};

#endif