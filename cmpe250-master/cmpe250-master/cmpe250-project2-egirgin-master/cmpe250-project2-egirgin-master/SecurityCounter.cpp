//
// Created by emregirgin on 30-Oct-18.
//

#include "SecurityCounter.h"

SecurityCounter::SecurityCounter() {
    this->is_empty= true;
    this->current_passenger = Passenger();
}
SecurityCounter::~SecurityCounter(){
}