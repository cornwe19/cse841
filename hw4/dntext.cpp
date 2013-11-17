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
      // Training phase
      XArea  X( xResponseSize, VOCAB_SIZE );
      ZArea  Z( FA_STATES );
      YArea  Y( settings.numYNeurons, X.response, xResponseSize, Z.response, FA_STATES );

       // Y's response is the size of X + the size of Z
      Z.setY( Y.response, xResponseSize + FA_STATES );
      X.setY( Y.response, xResponseSize + FA_STATES );

      unsigned lastWordId = 0;
      unsigned trainState = 0;

      ifstream trainingFile( settings.listingFile );
      char fileName[FILENAME_MAX];
      while ( trainingFile.getline( fileName, FILENAME_MAX ) ) {
         // Clear out Z to start each file
         Vectors::fill( Z.response, 0.0, FA_STATES );
         trainState = 0;

         Vocabulary vocab( fileName );
         while( ( lastWordId = vocab.nextWordId() ) != VOCAB_SIZE ) {
            for ( unsigned d = 0; d < SAMPLE_DURATION; d++ ) {
               if ( d == 1 ) {
                  results << "Transitioning: " << trainState << " -> ";
                  trainState = Transitions::getNextState( Z.getResponseState(), X.getResponseId() );
                  results << trainState << endl;
               }

               X.computePreresponse( lastWordId );
               Y.computePreresponse();
               Z.computePreresponse( trainState );

               X.update();
               Y.update();
               Z.update();
            }
         }
      }

      ofstream database( settings.networkFile, ios::binary | ios::out );
      X.writeToDatabase( database );
      Z.writeToDatabase( database );
      Y.writeToDatabase( database );
      database.close();

      char fileXY[FILENAME_MAX];
      sprintf( fileXY, "%s-xy.pgm", settings.outputFile );
      Y.saveNeuronBank( fileXY, 'X', 2 );
      
      char fileZY[FILENAME_MAX];
      sprintf( fileZY, "%s-zy.pgm", settings.outputFile );
      Y.saveNeuronBank( fileZY, 'Z', 2 );

      char fileAY[FILENAME_MAX];
      sprintf( fileAY, "%s-ay.txt", settings.outputFile );
      Y.saveAges( fileAY );

      char fileYZ[FILENAME_MAX];
      sprintf( fileYZ, "%s-yz.pgm", settings.outputFile );
      Z.saveNeuronBank( fileYZ, 2 );

      char fileYX[FILENAME_MAX];
      sprintf( fileYX, "%s-yx.pgm", settings.outputFile );
      X.saveNeuronBank( fileYX, 2 );
      
   } else {
      // Testing phase
      //  - Note thinking is handled automatically by passing '0' IDs into X
      ifstream database( settings.networkFile, ios::binary | ios::in );
      XArea  X( database );
      ZArea  Z( database );
      YArea  Y( database, X.response, Z.response ); 
      database.close();

      X.setY( Y.response );
      Z.setY( Y.response );

      unsigned   lastWordId = 0;
      const bool frozen = true;

      ifstream testingFile( settings.listingFile );
      char fileName[FILENAME_MAX];
      while ( testingFile.getline( fileName, FILENAME_MAX ) ) {

         Vocabulary vocab( fileName );
         while( ( lastWordId = vocab.nextWordId() ) != VOCAB_SIZE ) {
            for ( unsigned d = 0; d < SAMPLE_DURATION; d++ ) {
               bool wasSupervised = X.computePreresponse( lastWordId );
               Y.computePreresponse( frozen );
               Z.computePreresponse();

               X.update( wasSupervised );
               Y.update( frozen );
               Z.update( frozen );

               if ( d == 0 ) {
                  results << "Transitioning: " << Z.getResponseState() << " -> ";
               } else {
                  results << Z.getResponseState() << endl;
               }
            }
         }
      }
   }

   results.close();

   return 0;
}

