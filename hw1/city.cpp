#include "city.h"
#include "connection.h"

City::City( FILE* file ) {
   _name = new char[128];
   fscanf( file, "%s (%d,%d)\n", _name, &_x, &_y );

   _connections = new vector<Connection*>();
}


City::~City() {
   delete [] _name;
   _name = NULL;

   for ( int i = 0; i < _connections->size(); i++ ) {
      delete _connections->at(i);
   }

   delete _connections;
}

int City::distanceTo( City* other ) {
   return 4 * sqrt( pow( _x - other->getX(), 2 ) + pow( _y - other->getY(), 2 ) );
}

int City::flightTimeInMinutes( City* other ) {
   int takeOffLanding = 20;
   float airSpeedMph = 450.0f;
   return ( distanceTo( other ) / airSpeedMph ) * 60 + takeOffLanding;
}

int City::terminalWaitInMitues( City* other ) {
   int waitHours = 3;
   if ( isHub() ) {
      waitHours--;
   }

   if ( other->isHub() ) {
      waitHours--;
   }

   return waitHours * 60;
}


float City::computeHeuristic( City* dest, float costPerHour, float leastCostPerMile ) {
   return leastCostPerMile * distanceTo( dest ) + totalTimeCost( dest, costPerHour );
}

float City::totalTimeCost( City* dest, float costPerHour ) {
   return ( costPerHour / 60.0f ) * ( terminalWaitInMitues( dest ) + flightTimeInMinutes( dest ) );
}

void City::addConnection( int cost, City* city ) {
   Connection* connection = new Connection( cost, this, city );
   _connections->push_back( connection );
}
