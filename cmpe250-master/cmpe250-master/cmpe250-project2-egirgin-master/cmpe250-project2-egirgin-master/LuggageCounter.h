//
// Created by emregirgin on 08-Nov-18.
//

#ifndef AIRPORTSIMULATOR_LUGGAGECOUNTER_H
#define AIRPORTSIMULATOR_LUGGAGECOUNTER_H

#include "Passenger.h"

class LuggageCounter {
public:
    Passenger current_passenger;
    bool is_empty;
    LuggageCounter();
    ~LuggageCounter();
};


#endif //AIRPORTSIMULATOR_LUGGAGECOUNTER_H
