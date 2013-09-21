#include <vector>
#include <cmath>

using std::vector;

class City;

typedef struct Connection {
   int cost;
   City* toCity;

   Connection( int cost, City* toCity ) : 
      cost( cost ), toCity( toCity ) {}
} Connection;

class City {
   public:
      City( FILE* file ) {
         _name = new char[128];
         fscanf( file, "%s (%d,%d)\n", _name, &_x, &_y );

         _connections = new vector<Connection*>();
      }
      
      ~City() {
         delete [] _name;
         _name = NULL;

         for ( int i = 0; i < _connections->size(); i++ ) {
            delete _connections->at(i);
         }

         delete _connections;
      }

      int getX() {
         return _x;
      }

      int getY() {
         return _y;
      }

      char* getName() {
         return _name;
      }

      bool isHub() {
         return _name[0] == '*';
      }

      int distanceTo( City* other ) {
         return 4 * sqrt( pow( _x - other->getX(), 2 ) + pow( _y - other->getY(), 2 ) );
      }

      int flightTimeInMinutes( City* other ) {
         int takeOffLanding = 20;
         float airSpeedMph = 450.0f;
         return ( distanceTo( other ) / airSpeedMph ) * 60 + takeOffLanding;
      }

      int terminalWaitInMitues( City* other ) {
         int waitHours = 3;
         if ( isHub() ) {
            waitHours--;
         }

         if ( other->isHub() ) {
            waitHours--;
         }

         return waitHours * 60;
      }

      void addConnection( int cost, City* city ) {
         Connection* connection = new Connection( cost, city );
         _connections->push_back( connection );
      }

      vector<Connection*>* getConnections() {
         return _connections;
      }

   private:
      char* _name;
      int   _x, _y;

      vector<Connection*> *_connections;
};
