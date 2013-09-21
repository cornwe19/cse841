#include <iostream>
#include <fstream>
#include <sstream>
#include "city.h"
#include <vector>;

using namespace std;

int main( int argc, char** argv ) {
   if ( argc < 3 ) {
      cerr << "Usage: findRoute CITY_ORIGIN CITY_DEST COST_PER_HOUR" << endl;
      return -1;
   }
   
   vector<City*> cityList;

   FILE *cities = fopen( "data/cities.txt", "r+" );
   while ( !feof( cities ) ) {
      City *c = new City( cities );
      cout << c->getName() << " at (" << c->getX()  << "," << c->getY() << ") is " << ( c->isHub() ? "" : "not " ) << "a hub"  << endl;
      cityList.push_back( c );
   }
   fclose( cities );

   cout << "List is " << cityList.size() << " elements" <<  endl;
   
   int fromCity = 0;
   int toCity = 0;
   int cost;
   string line;

   ifstream connections( "data/flightCharges.txt" );
   while( getline( connections, line ) ) {
      istringstream currentLine( line );
      while ( currentLine ) {
         currentLine >> cost;

         if ( cost > 0 ) {
            cityList[fromCity]->addConnection( cost, cityList[toCity] );
         }

         toCity++;
      }

      toCity=0;
      fromCity++;
      line.clear();
   }

   cout << cityList[0]->getName() << " has " << cityList[0]->getConnections()->size() << " connections" << endl;
}
