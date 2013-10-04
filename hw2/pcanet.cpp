#include <stdio.h>
#include <stdlib.h>
#include <fstream>
#include "settings.h"
#include "listing.h"

#define IMAGE_SIZE 88*64

using namespace std;

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
   printf( "Generating mean image for class 0\n" );

   char* mean = new char[IMAGE_SIZE];
   char* x    = new char[IMAGE_SIZE];

   vector<char*>* class0 = listing.getClass( 0 );

   ifstream current( class0->at( 0 ), ios::binary );
   current.read( mean, IMAGE_SIZE );
   current.close();

   for ( unsigned gen = 1; gen < class0->size(); gen++ ) {
      current.open( class0->at( gen ), ios::binary );
      current.read( x, IMAGE_SIZE );

      float oneOverN = ( (float)gen / (float)( gen + 1 ) );
      for ( int i = 0; i < IMAGE_SIZE; i++ ) {
         mean[i] =  (char)( oneOverN * mean[i] + oneOverN * x[i] );
      }

      current.close();
   }
   
   printf( "Saving image to meanout.raw\n" );
   ofstream meanOut( "meanout.raw", ios::binary );
   meanOut.write( mean, IMAGE_SIZE );
   meanOut.close();

   return 0;
}

