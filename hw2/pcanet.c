#include <stdio.h>
#include <stdlib.h>

void usage( char** argv ) {
   fprintf( stderr, "Usage: %s [-l NUM_EPOCHS] -f FILE_NAME_LIST -d NETWORK_FILE -o OUTPUT_FILE\n", argv[0] );
   exit( 1 );
}

int main( int argc, char** argv ) {
   char* filesListFile = NULL;
   char* networkFile = NULL;
   char* outputFile = NULL;
   int   numEpochs = 0;

   // Parse commandline options
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
               networkFile = value;
               break;
            case 'f':
               filesListFile = value;
               break;
            case 'l':
               numEpochs = atoi( value );
               break;
            case 'o':
               outputFile = value;
               break;
            default:
               usage( argv );
               break;
         }
      }
   }

   if ( filesListFile == NULL || networkFile == NULL || outputFile == NULL ) {
      usage( argv );
   }


   printf( "Received the following parameters\n" );
   printf( "- network file: %s\n", networkFile );
   printf( "- file list: %s\n", filesListFile );
   printf( "- output file: %s\n", outputFile );
   printf( "- numEpochs: %d\n", numEpochs );

   return 0;
}

