#include <iostream>
#include <fstream>
#include <sstream>
#include "city.h"
#include "connection.h"
#include "city_comparator.h"
#include <vector>
#include <set>
#include <algorithm>
#include <iomanip>

using namespace std;

int main( int argc, char** argv ) {
   if ( argc < 3 ) {
      cerr << "Usage: findRoute CITY_ORIGIN CITY_DEST COST_PER_HOUR" << endl;
      return -1;
   }
   
   vector<City*> cityList;

   FILE *cities = fopen( "cities.txt", "r+" );
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

   ifstream connections( "flightCharges.txt" );
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

   int startIndex = atoi( argv[1] );
   int destIndex  = atoi( argv[2] );

   if ( startIndex >= cityList.size() || startIndex < 0 ||
        destIndex  >= cityList.size() || destIndex  < 0 ) {
      cerr << "Start city and end city must be integers in the range [0.." << cityList.size() << ")" << endl;
      return -1;
   }

   City* start    = cityList[startIndex];
   City* dest     = cityList[destIndex];
   float timeCost = atof( argv[3] );

   set<City*, CityComparator> openConnections;
  
   for ( int i = 0; i < cityList.size(); i++ ) {
      if ( cityList[i] == start ) {
         cityList[i]->cost = 0.0f;
      }

      openConnections.insert( cityList[i] );
   }

   while( !openConnections.empty() ) {
      City* current = ( *openConnections.begin() );

      if ( current == dest ) {
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
   
   int totalTime = 0;
   float totalCost = 0.0f;
   for ( int i = 0; i < results.size(); i++ ) {
      City *from = results[i]->getFromCity();
      City *to   = results[i]->getToCity();
      
      int nextTotalTime = totalTime + from->terminalWaitInMitues( to ) + from->flightTimeInMinutes( to );
      
      printf( "%d. %s - %s %d:%02d %d:%02d $%.2f\n", i + 1, from->getName(), to->getName(),
         totalTime / 60, totalTime % 60, nextTotalTime / 60, nextTotalTime % 60,
         results[i]->totalCost( timeCost ) );
      
      totalTime =  nextTotalTime;
      totalCost += results[i]->totalCost( timeCost );
   }

   printf( "Total Cost: $%.2f\n", totalCost );
}
