#ifndef CITY_H
#define CITY_H

#include <vector>
#include <cmath>

using std::vector;

class Connection;

class City {
   public:
      City( FILE* file );
      ~City();

      int   getX() { return _x; }
      int   getY() { return _y; }
      char* getName() { return _name;}
      bool  isHub() { return _name[0] == '*'; }

      int   distanceTo( City* other );
      int   flightTimeInMinutes( City* other );
      int   terminalWaitInMitues( City* other );
      float computeHeuristic( City* dest, float costPerHour, float leastCostPerMile );
      float totalTimeCost( City* dest, float costPerHour );
      void  addConnection( int cost, City* city );

      vector<Connection*>* getConnections() { return _connections; }

      // For graph search
      bool        isKnown;
      float       cost;
      Connection* link;

   private:
      char* _name;
      int   _x, _y;

      vector<Connection*> *_connections;
};

#endif
