#pragma once
#ifndef MEAT_WAGONS_TIME_H
#define MEAT_WAGONS_TIME_H

class Time {
    private:
        int hour, minute, second;

    public:
        Time() : hour(0), minute(0), second(0) {}
        Time(const int hour, const int minute, const int second) : hour(hour), minute(minute), second(second) {}

        int getHour() const;
        int getMinute() const;
        int getSecond() const;
        
        void setHour(const int hour);
        void setMinute(const int minute);
        void setSecond(const int second);

        int toSeconds() const;
        bool operator<(const Time &time) const;
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

bool Time::operator<(const Time &time) const {
    if(this->hour == time.hour)
        if(this->minute == time.minute)
            if(this->second == time.second) return false;
            else return this->second < time.second;
        else return this->minute < time.minute;
    else return this->hour < time.hour;
}

#endif //MEAT_WAGONS_TIME_H
