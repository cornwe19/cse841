#include <iostream>
#include <fstream>
#include <sstream>
#include "city.h"
#include "connection.h"
#include "city_comparator.h"
#include <vector>
#include <set>
#include <algorithm>

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

   // TODO: check bounds
   City* start    = cityList[ atoi( argv[1] ) ];
   City* dest     = cityList[ atoi( argv[2] ) ];
   float timeCost = atof( argv[3] );

   set<City*, CityComparator> openConnections;
  
   for ( int i = 0; i < cityList.size(); i++ ) {
      if ( cityList[i] == start ) {
         // cout << "Setting " << cityList[i]->getName() << " as start city" << endl;
         cityList[i]->cost = 0.0f;
      }

      openConnections.insert( cityList[i] );
   }

   while( !openConnections.empty() ) {
      City* current = ( *openConnections.begin() );
      // cout << "Testing " << current->getName() << endl;

      if ( current == dest ) {
         // cout << current->getName() << " is destination. Exiting" << endl;
         break;
      }
      
      openConnections.erase( current );
      current->isKnown = true;

      vector<Connection*>* nextConnections = current->getConnections();

      for ( int i = 0; i < nextConnections->size(); i++ ) {
         Connection* con = nextConnections->at(i);
         City* toCity = con->getToCity();

         float conCost = current->cost + con->totalCost( timeCost ) + toCity->computeHeuristic( dest, timeCost, leastCostPerMile ); 
         
         if ( !toCity->isKnown && conCost < toCity->cost ) {
            // cout << " - Updating connection " << current->getName() << "->" << toCity->getName() << " $" << conCost << endl;
            openConnections.erase( toCity );

            toCity->cost = conCost;
            toCity->link = con;

            openConnections.insert( toCity );
         }
      }
   }
  
   vector<Connection*> results;
   City* last = dest;
   while ( true ) {
      if ( last->link == NULL ) {
         break;
      }

      results.insert( results.begin(), last->link );
      last = last->link->getFromCity();
   }

   for ( int i = 0; i < results.size(); i++ ) {
      City *from = results[i]->getFromCity();
      City *to   = results[i]->getToCity();
      cout << from->getName() << " - " << to->getName() << " " << from->terminalWaitInMitues( to ) + from->flightTimeInMinutes( to ) << " $" << results[i]->totalCost( timeCost ) << endl;
   }
}
