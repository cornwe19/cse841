#include <iostream>
#include <fstream>
#include <sstream>
#include "city.h"
#include <vector>
#include <algorithm>

using namespace std;

void showFirst3Connections( City* local ) {
   vector<Connection*>* c = local->getConnections();
   int numConnections = min( 5, (int)c->size() );
   for ( int i = 0; i < numConnections; i++ ) {
      City* other = c->at(i)->toCity;
      
      cout << local->getName() << " to " << other->getName() << " is " << local->distanceTo( other ) << " miles"\
         " (takes " << local->flightTimeInMinutes( other ) + local->terminalWaitInMitues( other ) << " minutes)" << endl;
   }

   cout << endl;
}

int main( int argc, char** argv ) {
   if ( argc < 3 ) {
      cerr << "Usage: findRoute CITY_ORIGIN CITY_DEST COST_PER_HOUR" << endl;
      return -1;
   }
   
   vector<City*> cityList;

   FILE *cities = fopen( "data/cities.txt", "r+" );
   while ( !feof( cities ) ) {
      City *c = new City( cities );
      cityList.push_back( c );
   }
   fclose( cities );
   
   int fromCity = 0;
   int toCity = 0;
   int cost;
   string line;

   float leastCostPerMile = -1;

   ifstream connections( "data/flightCharges.txt" );
   while( getline( connections, line ) ) {
      istringstream currentLine( line );
      while ( currentLine >> cost ) {
         if ( cost > 0 ) {
            cityList[fromCity]->addConnection( cost, cityList[toCity] );

            float costPerMile = ((float)cost / (float)cityList[fromCity]->distanceTo( cityList[toCity] ));
            if ( leastCostPerMile < 0 || costPerMile < leastCostPerMile ) {
               leastCostPerMile = costPerMile;
            }
         }

         toCity++;
      }

      toCity=0;
      fromCity++;
      line.clear();
   }
  
   connections.close();

   cout << "Least cost per mile on map is " << leastCostPerMile << endl << endl;

   for ( int i = 0; i < 5; i++ ) {
      showFirst3Connections( cityList[i] );
   }
}
