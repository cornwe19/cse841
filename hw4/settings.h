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
   bool      isTraining;

   Settings() {
      listingFile = NULL;
      networkFile = NULL;
      outputFile  = NULL;
      isTraining  = false;
      numYNeurons = 0;
   }

   void load( int argc, char** argv, char** error ) {
      for ( int i = 1; i < argc; i++ ) {
         char* option = argv[i];
         int n = 0;

         if ( option[0] == '-' ) {
            switch( option[1] ) {
               case 'd':
                  if ( ++i < argc ) {
                     networkFile = argv[i];
                  } else {
                     err( argv[0], error );
                     return;
                  }
                  break;
               case 'f':
                  if ( ++i < argc ) {
                     listingFile = argv[i];
                  } else {
                     err( argv[0], error );
                     return;
                  }
                  break;
               case 'l':
                  isTraining = true;
                  break;
               case 'o':
                  if ( ++i < argc ) {
                     outputFile = argv[i];
                  } else {
                     err( argv[0], error );
                     return;
                  }
                  break;
               case 'Y':
                  if ( ++i < argc ) {
                     n = atoi( argv[i] );
                     numYNeurons = n*n; // n^2
                  } else {
                     err( argv[0], error );
                     return;
                  }
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
             ( !isTraining || numYNeurons > 0 );
   }

private:
   void err( char* progName, char** error ) {
       *error = new char[ERROR_MAX];
       sprintf( *error, "Usage: %s [-l -Y NUM_NEURONS] -f FILE_NAME_LIST -d NETWORK_FILE -o OUTPUT_FILE\n - Note: if training is set, neurons must also be\n", progName );
   }
};

#endif
