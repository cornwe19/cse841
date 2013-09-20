#include <iostream>
#include <fstream>
#include "city.h"

using namespace std;

int main( int argc, char** argv ) {
   if ( argc < 3 ) {
      cerr << "Usage: findRoute CITY_ORIGIN CITY_DEST COST_PER_HOUR" << endl;
      return -1;
   }
   
   FILE *cities = fopen( "data/cities.txt", "r+" );
   while ( !feof( cities ) ) {
      City c( cities );
      cout << c.getName() << " at (" << c.getX()  << "," << c.getY() << ") is " << ( c.isHub() ? "" : "not " ) << "a hub"  << endl;
   }
   
   fclose( cities );
}
