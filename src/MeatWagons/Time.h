#pragma once
#ifndef MEAT_WAGONS_TIME_H
#define MEAT_WAGONS_TIME_H

class Time {
    private:
        int hour, minute, second;

    public:
        Time() {}
        Time(int hour, int minute, int second) : hour(hour), minute(minute), second(second) {}

        int getHour() const;
        void setHour(const int hour);

        int getMinute() const;
        void setMinute(const int minute);

        int getSecond() const;
        void setSecond(const int second);

        int toSeconds() const;
};

int Time::getHour() const {
    return hour;
}

void Time::setHour(const int hour) {
    Time::hour = hour;
}

int Time::getMinute() const {
    return minute;
}

void Time::setMinute(const int minute) {
    Time::minute = minute;
}

int Time::getSecond() const {
    return second;
}

void Time::setSecond(const int second) {
    Time::second = second;
}

int Time::toSeconds() const {
    return second + minute*60 + hour*3600;
}

#endif //MEAT_WAGONS_TIME_H