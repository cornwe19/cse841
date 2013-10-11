#ifndef SETTINGS_H
#define SETTINGS_H

#include <stdlib.h>
#include <stdio.h>

#define ERROR_MAX 128

class Settings {
public:
   char* listingFile;
   char* networkFile;
   char* outputFile;
   int   numEpochs;
   char* testingFile;

   Settings() {
      listingFile = NULL;
      networkFile = NULL;
      outputFile  = NULL;
      numEpochs   = 1;
      testingFile = NULL;
   }

   void load( int argc, char** argv, char** error ) {
      if ( argc % 2 != 1 ) {
         err( argv[0], error );
      }

      int i = 1;
      for ( ; i < argc; i += 2 ) {
         char* option = argv[i];
         char* value  = argv[i+1];

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
               case 't':
                  testingFile = value;
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
             outputFile  != NULL;
   }

private:
   void err( char* progName, char** error ) {
       *error = new char[ERROR_MAX];
       sprintf( *error, "Usage: %s [-l NUM_EPOCHS] -f FILE_NAME_LIST -d NETWORK_FILE -o OUTPUT_FILE\n", progName );
   }

};

#endif
