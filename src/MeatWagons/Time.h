#pragma once
#ifndef MEAT_WAGONS_TIME_H
#define MEAT_WAGONS_TIME_H


class Time {
    private:
        int hour, minute, second;

    public:
        Time() { }
        Time(int hour, int minute, int second) {
            this->hour = hour;
            this->minute = minute;
            this->second = second;
        }

    const int getHour() const;
    const void setHour(int hour);
    const int getMinute() const;
    const void setMinute(int minute);
    const int getSecond() const;
    const void setSecond(int second);
    const int toSeconds();

};

const int Time::getHour() const {
    return hour;
}

const void Time::setHour(int hour) {
    Time::hour = hour;
}

const int Time::getMinute() const {
    return minute;
}

const void Time::setMinute(int minute) {
    Time::minute = minute;
}

const int Time::getSecond() const {
    return second;
}

const void Time::setSecond(int second) {
    Time::second = second;
}

const int Time::toSeconds() {
    return second + minute*60 + hour*3600;
}

#endif //MEAT_WAGONS_TIME_H