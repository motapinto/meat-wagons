#pragma once
#ifndef MEAT_WAGONS_TIME_H
#define MEAT_WAGONS_TIME_H
#include <iomanip>
using namespace std;

class Time {
    private:
        int hour, minute, second;

    public:
        Time() : hour(0), minute(0), second(0) {}
        Time(const int hour, const int minute, const int second) : hour(hour), minute(minute), second(second) {}

        int getHour() const;
        int getMinute() const;
        int getSecond() const;

        bool operator<(const Time &time) const;
        Time operator+(const Time &time) const;

        friend std::ostream &operator<<( std::ostream &output, const Time &time) {
            output << setfill('0') << left << setw(2) << time.getHour() << ":"
                   << setfill('0') << left << setw(2) << time.getMinute() << ":"
                   << setfill('0') << left << setw(2) << time.getSecond();
            return output;
        }
};

int Time::getHour() const {
    return this->hour;
}

int Time::getMinute() const {
    return this->minute;
}

int Time::getSecond() const {
    return this->second;
}

bool Time::operator<(const Time &time) const {
    if(this->hour == time.hour)
        if(this->minute == time.minute)
            if(this->second == time.second) return false;
            else return this->second < time.second;
        else return this->minute < time.minute;
    else return this->hour < time.hour;
}

Time Time::operator+(const Time &time) const {
    Time added = *this;
    added.second += time.second;
    added.minute += time.minute + (this->second / 60);
    added.hour += time.hour + (this->minute % 60);
    added.second %= 60;
    added.minute %= 60;

    return added;
}

#endif //MEAT_WAGONS_TIME_H