#include <stdio.h>
#include <fstream>
#include <cmath>
#include <climits>
#include <numeric>
#include <cfloat>
#include "settings.h"
#include "listing.h"
#include "image.h"

#define IMAGE_SIZE 64*88

using namespace std;

void amnesic( unsigned t, float* w1, float* w2 ) {
   float t1 = 20;
   float t2 = 500;
   float r  = 1000;
   float mu = 0;

   if ( t >= t1 && t < t2 ) {
      mu = 2.0f * ( t - t1 ) / ( t2 - t1 );
   } else if ( t >= t2 ) {
      mu = 2.0f + ( t - t2 ) / r;
   }

   *w1 = ( t - 1 - mu ) / t;
   *w2 = ( 1 + mu ) / t;
}

double sum_square( double x, double y ) { return x + y*y; }

double vecLen( double* vec, size_t size ) {
   return sqrt( accumulate( vec, vec + size, 0, sum_square ) );
}

void scaleTo255( char* dest, double* src ) {
   for ( int i = 0; i < IMAGE_SIZE; i++ ) {
      dest[i] = ( 255 * ( src[i] - INT_MIN ) ) / ( INT_MAX - INT_MIN );
   }
}

void dumpEigFaces( unsigned gen, char* scratch, vector<double*> data ) {
   for ( unsigned i = 0; i < data.size(); i++ ) {
      scaleTo255( scratch, data[i] );

      char outName[32];
      sprintf( outName, "eig%d-%d.raw", i, gen );
      ofstream viOut( outName, ios::binary );
      viOut.write( scratch, IMAGE_SIZE );
      viOut.close();
   }
}

int main( int argc, char** argv ) {
   Settings settings;

   char* error = NULL;
   settings.load( argc, argv, &error );

   if ( error != NULL ) {
       fprintf( stderr, "%s", error );
       exit( 1 );
   }

   Listing training;
   training.load( settings.listingFile );
  
   printf( "Loaded %d training images\n", training.size() );

   if ( settings.testingFile != NULL ) {
      Listing testing;
      testing.load( settings.testingFile );

      int numRight = 0;
      int total = 0;

      for ( unsigned t = 0; t < testing.size(); t++ ) {
         Image test( testing[t] );
         double leastDistance = DBL_MAX;

         for ( unsigned i = 0; i < training.size(); i++ ) {
            Image *current = new Image( training[i] );
            double dist = test.euclideanDistanceTo( current );
            
            if ( dist < leastDistance ) {
               if ( test.bestMatch != NULL ) {
                  delete test.bestMatch;
               }

               leastDistance = dist;
               test.bestMatch = current;
            }
         }

         if ( !strcmp( test.bestMatch->getClassName(), test.getClassName() ) ) {
            numRight++;
         }
         total++;
      }

      printf( "Classifier was right %.2f of the time\n", (float)numRight / (float)total );
   }
   
/*
   char* x    = new char[IMAGE_SIZE];
   int* mean  = new int[IMAGE_SIZE];
   int* u     = new int[IMAGE_SIZE];

   vector<int*> v; // Holds our eigen vectors
   unsigned k = 1;

   vector<char*>* clazz = training.getClass( 1 );

   ifstream current( clazz->at( 0 ), ios::binary );
   current.read( x, IMAGE_SIZE );
   current.close();

   for ( int i = 0; i < IMAGE_SIZE; i++ ) {
      mean[i] = (int)x[i];
   }
   
   int n = 2;
   float w1 = 0;
   float w2 = 0;

   for ( int e = 0; e < settings.numEpochs; e++ ) {
      for ( unsigned t = 0; t < clazz->size(); t++ ) {
         current.open( clazz->at( t ), ios::binary );
         current.read( x, IMAGE_SIZE );

         float tOverN = ( (float)( t ) / (float)( t + 1 ) );
         float oneOverN = ( 1.0f / (float)( t + 1 ) );
         for ( int i = 0; i < IMAGE_SIZE; i++ ) {
            mean[i] = tOverN * mean[i] + oneOverN * x[i];
            u[i] = x[i] - mean[i];
         }

         amnesic( n++, &w1, &w2 );

         for ( unsigned i = 0; i <= min( t, k ); i++ ) {

            if ( i == t ) {
               int* vi = new int[IMAGE_SIZE];
               for ( int j = 0; j < IMAGE_SIZE; j++ ) {
                  vi[j] = u[j];
               }
               v.push_back( vi );
               k++;
            } else {
               float lvi = vecLen( v[i], IMAGE_SIZE );

               int yi = 0;
               for ( int j = 0; j < IMAGE_SIZE; j++ ) {
                  yi += u[j] * ( v[i][j] / lvi );
               }
               
               for ( int j = 0; j < IMAGE_SIZE; j++ ) {
                  // Update current principal component estimate
                  v[i][j] = w1 * v[i][j] + w2 * yi * u[j];
               }

               lvi = vecLen( v[i], IMAGE_SIZE ); // Update norm(v[i])
               for ( int j = 0; j < IMAGE_SIZE; j++ ) {
                  // Update residual for next component
                  u[j] = u[j] - yi * (v[i][j] / lvi);
               }
            }
         }
      }

      current.close();
      
      if ( e == 0 ) {
         dumpEigFaces( e, x, v );
      }
   }

   dumpEigFaces( settings.numEpochs, x, v );

   ofstream meanOut( "meanout.raw", ios::binary );
   scaleTo255( x, mean );
   meanOut.write( x, IMAGE_SIZE );
   meanOut.close();
*/

   return 0;
}

