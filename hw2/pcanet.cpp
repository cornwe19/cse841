#include <stdio.h>
#include <stdlib.h>
#include "settings.h"
#include "listing.h"

void usage( char** argv ) {
   fprintf( stderr, "Usage: %s [-l NUM_EPOCHS] -f FILE_NAME_LIST -d NETWORK_FILE -o OUTPUT_FILE\n", argv[0] );
   exit( 1 );
}

void parseCmdArgs( int argc, char** argv, Settings* settings ) {
   if ( argc % 2 != 1 ) {
      usage( argv );
   }

   int i = 1;
   for ( ; i < argc; i += 2 ) {
      char* option = argv[i];
      char* value  = argv[i+1];

      if ( option[0] == '-' ) {
         switch( option[1] ) {
            case 'd':
               settings->networkFile = value;
               break;
            case 'f':
               settings->listingFile = value;
               break;
            case 'l':
               settings->numEpochs = atoi( value );
               break;
            case 'o':
               settings->outputFile = value;
               break;
            default:
               usage( argv );
               break;
         }
      }
   }

   if ( !settings->areValid() ) {
      usage( argv );
   }
}

int main( int argc, char** argv ) {
   Settings settings;

   parseCmdArgs( argc, argv, &settings );

   Listing listing;
   listing.load( &settings );

   printf( "Loaded %d classes\n", listing.getNumClasses() );

   printf( "Class 1:\n" );
   for ( unsigned i = 0; i < listing.getClass( 0 )->size(); i++ ) {
      printf( " - %s\n", listing.getClass( 0 )->at( i )->c_str() );
   }

   return 0;
}

