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
#include "y_area.h"

#define IMAGE_SIZE 64*88
#define SAMPLE_DURATION 3

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
      double Z[training.numClasses() + 1];
      YArea  Y( settings.numYNeurons, X, IMAGE_SIZE, Z, training.numClasses() + 1 );

      int t = 1;

      for ( unsigned image = 0; image < training.size(); image++ ) {
         
         for ( unsigned background = 0; background < SAMPLE_DURATION; background++ ) {
            
         }
         
         for ( unsigned sample = 0; sample < SAMPLE_DURATION; sample++ ) {
            
         }
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

