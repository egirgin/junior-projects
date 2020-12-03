//
// Created by emregirgin on 30-Oct-18.
//

#include "Passenger.h"
#include <iostream>

using namespace std;

bool Passenger::fly_first=false;

Passenger::Passenger() {
    this->arrival_time=0;
    this->boarding_time=0;
    this->luggage_time=0;
    this->security_time=0;
    this->vip=false;
    this->online_ticket=false;
}

Passenger::Passenger(int arrival_time, int boarding_time, int luggage_time, int security_time, char vip, char online_ticket) {
    this->arrival_time= arrival_time;
    this->boarding_time= boarding_time;
    this->luggage_time = luggage_time;
    this->security_time = security_time;
    if(vip=='V'){
        this->vip= true;
    }
    else{
        this->vip= false;
    }
    if(online_ticket=='L'){
        this->online_ticket= false;
    }
    else{
        this->online_ticket= true;
    }
}

Passenger::Passenger(const Passenger &passenger) {
    this->arrival_time = passenger.arrival_time;
    this->boarding_time = passenger.boarding_time;
    this->security_time = passenger.security_time;
    this->luggage_time = passenger.luggage_time;
    this->online_ticket = passenger.online_ticket;
    this->vip = passenger.vip;
}

Passenger &Passenger::operator=(const Passenger &passenger) {
    arrival_time = passenger.arrival_time;
    boarding_time = passenger.boarding_time;
    security_time = passenger.security_time;
    luggage_time = passenger.luggage_time;
    online_ticket = passenger.online_ticket;
    vip = passenger.vip;;
    return *this;
}

bool Passenger::operator<(const Passenger &passenger) const {
    if(!fly_first) {
        return this->arrival_time < passenger.arrival_time;
    }
    else {
        if (this->boarding_time == passenger.boarding_time) {
            return this->arrival_time < passenger.arrival_time;
        } else {
            return this->boarding_time < passenger.boarding_time;
        }
    }
}

bool Passenger::operator>(const Passenger &passenger) const {
    if (!fly_first) {
        return this->arrival_time > passenger.arrival_time;
    }
    else{
        if (this->boarding_time == passenger.boarding_time) {
            return this->arrival_time > passenger.arrival_time;
        } else {
            return this->boarding_time > passenger.boarding_time;
        }
    }
}

bool Passenger::operator==(const Passenger &passenger) const {
    if (!fly_first) {
        return this->arrival_time == passenger.arrival_time;
    }
    else{
        return this->boarding_time == passenger.boarding_time;
    }
}

bool Passenger::operator>=(const Passenger &passenger) const {
    if (!fly_first) {
        return this->arrival_time >= passenger.arrival_time;
    }
    else{
        if (this->boarding_time == passenger.boarding_time) {
            return this->arrival_time >= passenger.arrival_time;
        } else {
            return this->boarding_time >= passenger.boarding_time;
        }
    }
}

bool Passenger::operator<=(const Passenger &passenger) const {
    if (!fly_first) {
        return this->arrival_time <= passenger.arrival_time;
    }
    else{
        if (this->boarding_time == passenger.boarding_time) {
            return this->arrival_time <= passenger.arrival_time;
        } else {
            return this->boarding_time <= passenger.boarding_time;
        }
    }
}

bool Passenger::operator!=(const Passenger &passenger) const {
    if (!fly_first) {
        return this->arrival_time != passenger.arrival_time;
    }
    else{
        return this->boarding_time != passenger.boarding_time;
    }
}

Passenger::~Passenger() {
    this->arrival_time=0;
    this->boarding_time=0;
    this->vip= 0;
    this->online_ticket= 0;
    this->luggage_time=0;
    this->security_time=0;
}