//
// Created by emregirgin on 30-Oct-18.
//

#ifndef AIRPORTSIMULATOR_PASSANGER_H
#define AIRPORTSIMULATOR_PASSANGER_H

class Passenger{
public:
    static bool fly_first;
    int arrival_time;
    int boarding_time;
    int luggage_time;
    int security_time;
    bool online_ticket;
    bool vip;

public:
    Passenger();
    Passenger(int arrival_time, int boarding_time, int luggage_time, int security_time, char vip, char online_ticket);
    Passenger(const Passenger &passenger);
    Passenger& operator=(const Passenger &passenger);
    bool operator <(const Passenger& p2) const;
    bool operator >(const Passenger& passenger) const;
    bool operator ==(const Passenger& passenger) const;
    bool operator >=(const Passenger& passenger) const;
    bool operator <=(const Passenger& passenger) const;
    bool operator !=(const Passenger& passenger) const;
    ~Passenger();
};


#endif //AIRPORTSIMULATOR_PASSANGER_H
