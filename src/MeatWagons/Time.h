#pragma once
#ifndef MEAT_WAGONS_TIME_H
#define MEAT_WAGONS_TIME_H

class Time {
    private:
        int hour, minute, second;

    public:
        Time() {}
        Time(const int hour, const int minute, const int second) : hour(hour), minute(minute), second(second) {}

        int getHour() const;
        void setHour(const int hour);

        int getMinute() const;
        void setMinute(const int minute);

        int getSecond() const;
        void setSecond(const int second);

        int toSeconds() const;
};

int Time::getHour() const {
    return this->hour;
}

void Time::setHour(const int hour) {
    this->hour = hour;
}

int Time::getMinute() const {
    return this->minute;
}

void Time::setMinute(const int minute) {
    this->minute = minute;
}

int Time::getSecond() const {
    return this->second;
}

void Time::setSecond(const int second) {
    this->second = second;
}

int Time::toSeconds() const {
    return this->second + this->minute*60 + this->hour*3600;
}

#endif //MEAT_WAGONS_TIME_H