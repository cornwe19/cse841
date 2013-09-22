#ifndef CONNECTION_H
#define CONNECTION_H

#include "city.h"
#include <cfloat>

class Connection {

public:
   Connection( int cost, City* fromCity, City* toCity ) :
      _cost( cost ), _fromCity( fromCity ), _toCity( toCity ) {}

   float totalCost( float costPerHour ) {
      return _fromCity->totalTimeCost( _toCity, costPerHour ) + _cost;
   }

   City* getFromCity() {
      return _fromCity;
   }

   City* getToCity() {
      return _toCity;
   }


private:
   City* _fromCity;
   City* _toCity;
   int   _cost;
};

#endif
