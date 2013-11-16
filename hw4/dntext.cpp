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

   const unsigned xResponseSize = (unsigned)ceil( log( VOCAB_SIZE ) / log( 2 ) );

   ofstream results( settings.outputFile );

   if ( settings.isTraining ) {
      double X[xResponseSize];
      double XNorm[xResponseSize];
      ZArea  Z( FA_STATES );
      YArea  Y( settings.numYNeurons, XNorm, xResponseSize, Z.response, FA_STATES );

       // Y's response is the size of X + the size of Z
      Z.setY( Y.response, xResponseSize + FA_STATES );

      unsigned lastWordId = 0;
      unsigned trainState = 0;

      ifstream trainingFile( settings.listingFile );
      char fileName[FILENAME_MAX];
      while ( trainingFile.getline( fileName, FILENAME_MAX ) ) {
         printf( "Processing %s\n\n", fileName );

         // Clear out Z to start each file
         Vectors::fill( Z.response, 0.0, FA_STATES );
         trainState = 0;

         Vocabulary vocab( fileName );
         while( ( lastWordId = vocab.nextWordId() ) != VOCAB_SIZE ) {
            XArea::encodeId( lastWordId, X, xResponseSize );
            Vectors::norm( XNorm, X, xResponseSize );

            for ( unsigned d = 0; d < SAMPLE_DURATION; d++ ) {
               if ( d == 1 ) {
                  results << "Transitioning: " << trainState << " -> ";
                  trainState = Transitions::getNextState( Z.response, XArea::decodeId( X, xResponseSize ) );
                  results << trainState << endl;
               }

               Y.computePreresponse();
               Z.computePreresponse( trainState );

               Y.update();
               Z.update();
            }
         }
      }

      ofstream database( settings.networkFile, ios::binary | ios::out );
      Z.writeToDatabase( database );
      Y.writeToDatabase( database );
      database.close();
   } else {
      double X[xResponseSize];
      double XNorm[xResponseSize];
      
      ifstream database( settings.networkFile, ios::binary | ios::in );
      ZArea  Z( database );
      YArea  Y( database, XNorm, Z.response ); 
      database.close();

      Z.setY( Y.response );

      unsigned   lastWordId = 0;
      const bool frozen = true;

      ifstream testingFile( settings.listingFile );
      char fileName[FILENAME_MAX];
      while ( testingFile.getline( fileName, FILENAME_MAX ) ) {
         printf( "Processing %s\n\n", fileName );

         Vocabulary vocab( fileName );
         while( ( lastWordId = vocab.nextWordId() ) != VOCAB_SIZE ) {
            XArea::encodeId( lastWordId, X, xResponseSize );
            Vectors::norm( XNorm, X, xResponseSize );

            for ( unsigned d = 0; d < SAMPLE_DURATION; d++ ) {

               Y.computePreresponse( frozen );
               Z.computePreresponse();

               Y.update( frozen );
               Z.update( frozen );

               // TODO: debug
               unsigned zState = 0;
               for ( unsigned i = 1; i < FA_STATES; i++ ) {
                  if ( Z.response[i] > 0 ) {
                     zState = i;
                     break;
                  }
               }
               if ( d == 0 ) {
                  results << "Transitioning: " << zState << " -> ";
               } else {
                  results << zState << endl;
               }
               // TODO: debug
            }
         }
      }
   }

   results.close();

   return 0;
}

