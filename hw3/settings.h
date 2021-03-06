/**
* Dennis Cornwell
* Settings class used for loading/caching program cmd arguments
*  - Reads cmd arguments and helps determine their validity
**/

#ifndef SETTINGS_H
#define SETTINGS_H

#include <stdlib.h>
#include <stdio.h>

#define ERROR_MAX 128

class Settings {
public:
   char*     listingFile;
   char*     networkFile;
   char*     outputFile;
   unsigned  numYNeurons;
   unsigned  numEpochs;

   Settings() {
      listingFile = NULL;
      networkFile = NULL;
      outputFile  = NULL;
      numEpochs   = 0;
      numYNeurons = 0;
   }

   void load( int argc, char** argv, char** error ) {
      if ( argc % 2 != 1 ) {
         err( argv[0], error );
      }

      for ( int i = 1; i < argc; i += 2 ) {
         char* option = argv[i];
         char* value  = argv[i+1];
         int n = 0;

         if ( option[0] == '-' ) {
            switch( option[1] ) {
               case 'd':
                  networkFile = value;
                  break;
               case 'f':
                  listingFile = value;
                  break;
               case 'l':
                  numEpochs = atoi( value );
                  break;
               case 'o':
                  outputFile = value;
                  break;
               case 'Y':
                  n = atoi( value );
                  numYNeurons = n*n; // n^2
                  break;
               default:
                  err( argv[0], error );
                  break;
            }
         }
      }

      if ( !areValid() ) {
         err( argv[0], error );
      }
   }

   bool areValid() {
      return listingFile != NULL &&
             networkFile != NULL &&
             outputFile  != NULL && 
             // If we're testing num neurons must not be 0
             ( numEpochs == 0 || numYNeurons > 0 );
   }

private:
   void err( char* progName, char** error ) {
       *error = new char[ERROR_MAX];
       sprintf( *error, "Usage: %s [-l NUM_EPOCHS -Y NUM_NEURONS] -f FILE_NAME_LIST -d NETWORK_FILE -o OUTPUT_FILE\n - Note: if epochs are set, neurons must also be\n", progName );
   }

};

#endif
