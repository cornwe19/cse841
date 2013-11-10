/**
* Dennis Cornwell
* Developmental Network implementation with two modes:
*  - Training: Z is supervised, X is strictly input and Y is free to learn from both
*  - Testing: Z is free, X is strictly input and Y is locked (does not adapt, just updates response)
**/

#include <stdio.h>
#include <fstream>
#include <iomanip>
#include <cmath>
#include <climits>
#include <numeric>
#include <cfloat>
#include <string.h>
#include "settings.h"
#include "y_area.h"
#include "z_area.h"
#include "vectors.h"
#include "vocabulary.h"
#include "transitions.h"

#define SIGMA 6
#define SAMPLE_DURATION 2

using namespace std;

int main( int argc, char** argv ) {
   Settings settings;

   char* error = NULL;
   settings.load( argc, argv, &error );

   if ( error != NULL ) {
       fprintf( stderr, "%s", error );
       exit( 1 );
   }

   unsigned xNeurons = (unsigned)ceil( log( VOCAB_SIZE ) / log( 2 ) );
   printf( "Using X size %d\n", xNeurons ); 

   // Training mode
   if ( settings.isTraining ) {
      double X[xNeurons];
      double Z[FA_STATES];
      YArea  Y( settings.numYNeurons, X, VOCAB_SIZE, Z, FA_STATES );

      ifstream trainingFile( settings.listingFile );
      char fileName[FILENAME_MAX];
      while ( !trainingFile.eof() ) {
         trainingFile.getline( fileName, FILENAME_MAX );

         // Clear Z training before processing next file
         Vectors::fill( Z, 0.0, FA_STATES );
         Vocabulary vocab( fileName );
         while( vocab.encodeNextWord( X, xNeurons ) ) {
            printf( "X( %.0f, %.0f, %.0f, %.0f )\n", X[0], X[1], X[2], X[3] );

            for ( unsigned d = 0; d < SAMPLE_DURATION; d++ ) {
               Y.computePreresponse();
               Y.update();

               if ( d == 1 ) {
                  printf( "Transitioning: " );
                  Vectors::print( Z, FA_STATES );
                  printf( " -> " );
                  Transitions::updateState( Z, X, xNeurons );
                  Vectors::print( Z, FA_STATES );
                  printf( "\n" );
               }
            }
         }

         printf( "\n" );
      }

   } 
   // Testing mode
   else {
      // TODO: testing steps 
   }

   return 0;
}

