#include <stdio.h>
#include <stdlib.h>
#include <fstream>
#include <cmath>
#include <numeric>
#include "settings.h"
#include "listing.h"

#define IMAGE_SIZE 64*88

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

void amnesiac( unsigned i, float* w1, float* w2 ) {
   float t1 = 20;
   float t2 = 200;
   float r  = 2000;
   float mu = 0;

   if ( i > t1 && i <= t2 ) {
      mu = 2.0f * ( i - t1 ) / ( t2 - t1 );
   } else if ( i > t2 ) {
      mu = 2.0f + ( i - t2 ) / r;
   }

   *w1 = ( i - 1 - mu ) / i;
   *w2 = ( 1 + mu ) / i;
}

int sum_square( int x, int y ) { return x + y*y; }

float vecLen( char* vec, size_t size ) {
   return sqrt( accumulate( vec, vec + size, 0, sum_square ) );
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
   char* u    = new char[IMAGE_SIZE];

   vector<char*> v; // Holds our eigen vectors
   unsigned k = 1;

   vector<char*>* clazz = listing.getClass( 0 );

   ifstream current( clazz->at( 0 ), ios::binary );
   current.read( mean, IMAGE_SIZE );
   current.close();

   for ( int e = 0; e < settings.numEpochs; e++ ) {
      for ( unsigned t = 1; t < clazz->size(); t++ ) {
         current.open( clazz->at( t ), ios::binary );
         current.read( x, IMAGE_SIZE );

         float oneOverN = ( (float)t / (float)( t + 1 ) );
         for ( int i = 0; i < IMAGE_SIZE; i++ ) {
            mean[i] =  (char)( oneOverN * mean[i] + oneOverN * x[i] );
            u[i] = x[i] - mean[i];
         }

         if ( k == t ) {
            char* vi = new char[IMAGE_SIZE];
            memcpy( vi, u, IMAGE_SIZE );
            v.push_back( vi );
            k++;
         } else {
            float w1 = 0;
            float w2 = 0;
            amnesiac( t, &w1, &w2 );
            float lvi = vecLen( v[1], IMAGE_SIZE );
            for ( unsigned i = 1; i < min( t, k ); i++ ) {
               for ( int j = 0; j < IMAGE_SIZE; j++ ) {
                  // Update current principal component estimate
                  v[i][j] = w1 * v[i][j] + w2 * v[i][j] * u[i] * u[i] / lvi;
               }

               lvi = vecLen( v[i], IMAGE_SIZE );
               for ( int j = 0; j < IMAGE_SIZE; j++ ) {
                  // Update residual for next component
                  u[i] = u[i] - (u[i] * v[i][j] * v[i][j]) / (lvi * lvi);
               }
            }
         }

         current.close();
      }
   }

   printf( "Saving image to meanout.raw\n" );
   ofstream meanOut( "meanout.raw", ios::binary );
   meanOut.write( mean, IMAGE_SIZE );
   meanOut.close();
   
   for ( unsigned i = 0; i < v.size(); i++ ) {
      char outName[32];
      sprintf( outName, "eig%d.raw", i );
      ofstream viOut( outName, ios::binary );
      viOut.write( v[i], IMAGE_SIZE );
      viOut.close();
   }

   return 0;
}

