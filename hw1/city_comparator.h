#ifndef CITY_COMPARATOR_H
#define CITY_COMPARATOR_H

#include "city.h"
#include <string>

class CityComparator {
   
public:
   bool operator() ( City* lhs, City* rhs ) const {
      if ( lhs->cost != rhs->cost ) {
         return lhs->cost < rhs->cost;
      } else {
         return std::string( lhs->getName() ) < std::string( rhs->getName() );
      }
   }
};

#endif
