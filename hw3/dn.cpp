/**
* Dennis Cornwell
* ICCPCA net implementation with 2 distinct states, 1 for training and the other for testing
*  - Generates a series of MEF eigen vector images, a mean image, a database file and an output report
**/

#include <stdio.h>
#include <fstream>
#include <cmath>
#include <climits>
#include <numeric>
#include <cfloat>
#include <string.h>
#include "settings.h"
#include "listing.h"
#include "image.h"

#define IMAGE_SIZE 64*88

using namespace std;

int main( int argc, char** argv ) {
   Settings settings;

   char* error = NULL;
   settings.load( argc, argv, &error );

   if ( error != NULL ) {
       fprintf( stderr, "%s", error );
       exit( 1 );
   }

   ofstream out( settings.outputFile );

   // Training mode
   if ( settings.numEpochs > 0 ) {
      Listing training;
      training.load( settings.listingFile );
    
      out << "Read " << training.size() << " images" << endl;
      out << "Database file '" << settings.networkFile << "'" << endl;

      char   scratch[IMAGE_SIZE]; // Scratch buffer for conversion from double*
      double X[IMAGE_SIZE];
      double Y[settings.numYNeurons];
      double Z[training.numClasses()];

      int t = 1;
      double w1 = 0;
      double w2 = 0;

      for ( unsigned epoch = 0; epoch < settings.numEpochs; epoch++ ) {
         // Do DN things.
      }

      out.close();
   } 
   // Testing mode
   else {
      Listing testing;
      testing.load( settings.listingFile );
      
      // Do DN testing.
   }

   return 0;
}

