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
#include "x_area.h"
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

   unsigned xResponseSize = (unsigned)ceil( log( VOCAB_SIZE ) / log( 2 ) );

   // Training mode
   if ( settings.isTraining ) {
      XArea  X( xResponseSize, VOCAB_SIZE );
      double Z[FA_STATES];
      YArea  Y( settings.numYNeurons, X.response, xResponseSize, Z, FA_STATES );
      unsigned lastWordId = 0;

      X.setConnections( &lastWordId, Y.responseX );

      ifstream trainingFile( settings.listingFile );
      char fileName[FILENAME_MAX];
      while ( trainingFile.getline( fileName, FILENAME_MAX ) ) {
         // Clear Z training before processing next file
         Vectors::fill( Z, 0.0, FA_STATES );

         Vocabulary vocab( fileName );
         while( ( lastWordId = vocab.nextWordId() ) > 0 ) {
            Vectors::print( X.response, xResponseSize );
            printf( "\n" );

            for ( unsigned d = 0; d < SAMPLE_DURATION; d++ ) {
               X.computePreresponse();
               Y.computePreresponse();

               X.update();
               Y.update();

               if ( d == 1 ) {
                  printf( "Transitioning: " );
                  Vectors::print( Z, FA_STATES );
                  printf( " -> " );
                  Transitions::updateState( Z, X.getResponseId() );
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

