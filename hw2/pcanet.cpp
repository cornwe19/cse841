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

void amnesic( unsigned t, double* w1, double* w2 ) {
   double t1 = 20;
   double t2 = 500;
   double r  = 1000;
   double mu = 0;

   if ( t >= t1 && t < t2 ) {
      mu = 2.0f * ( t - t1 ) / ( t2 - t1 );
   } else if ( t >= t2 ) {
      mu = 2.0f + ( t - t2 ) / r;
   }

   *w1 = ( t - 1 - mu ) / t;
   *w2 = ( 1 + mu ) / t;
}

double vecLen( double* vec, size_t size ) {
   double sum = 0;
   for ( unsigned i = 0; i < size; i++ ) {
      sum += ( vec[i] * vec[i] );
   }

   return sqrt( sum );
}

void scaleTo255( char* dest, double* src ) {
   for ( unsigned i = 0; i < IMAGE_SIZE; i++ ) {
      dest[i] = ( 255 * ( ((long long)src[i] - LLONG_MIN) / (LLONG_MAX - LLONG_MIN) ) );
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

   // Mean image
   double mean[IMAGE_SIZE];
   double u   [IMAGE_SIZE];
   char   x   [IMAGE_SIZE];
   
   int n = 2;
   double w1 = 0;
   double w2 = 0;
   const unsigned k = 5;

   vector<double*> v[training.numClasses()];

   for ( int e = 0; e < settings.numEpochs; e++ ) {
      for ( unsigned c = 0; c < training.numClasses(); c++ ) {
         imgClass_t* clazz = training.classAt( c );

         unsigned iterStart = 0;
         if ( e == 0 && c == 0 ) {
            Image init( clazz->ptr[0] );
            for ( int p = 0; p < IMAGE_SIZE; p++ ) {
               mean[p] = init[p];
            }

            iterStart = 1;
         }

         for ( unsigned i = iterStart; i < clazz->size; i++ ) {
            Image img( clazz->ptr[i] );

            double tOverN = ( (double)( i ) / (double)( i + 1 ) );
            double oneOverN = ( 1.0f / (double)( i + 1 ) );
            for ( int p = 0; p < IMAGE_SIZE; p++ ) {
               mean[p] = tOverN * mean[p] + oneOverN * img[p];
               u[p] = img[p] - mean[p];
            }
            
            amnesic( n++, &w1, &w2 );
            
            for ( unsigned comp = 0; comp < min( i, k ); comp++ ) {

               if ( v[c].size() == comp ) {
                  double* vi = new double[IMAGE_SIZE];
                  for ( int j = 0; j < IMAGE_SIZE; j++ ) {
                     vi[j] = u[j];
                  }

                  v[c].push_back( vi );
               } else {
                  
                  double lvi = vecLen( v[c][comp], IMAGE_SIZE );
                  
                  double yi = 0;
                  for ( int j = 0; j < IMAGE_SIZE; j++ ) {
                     yi += u[j] * v[c][comp][j] / lvi;
                  }

                  for ( int j = 0; j < IMAGE_SIZE; j++ ) {
                     // Update current principal component estimate
                     v[c][comp][j] = w1 * v[c][comp][j] + w2 * yi;
                  }

                  lvi = vecLen( v[c][comp], IMAGE_SIZE ); // Update norm(v[i])

                  for ( int j = 0; j < IMAGE_SIZE; j++ ) {
                     // Update residual for next component
                     u[j] = u[j] - yi * v[c][comp][j] / lvi;
                  }
               }
            }
         }
      }

      if ( e == 0 || e == settings.numEpochs - 1 ) {
         for ( unsigned i = 0; i < training.numClasses(); i++ ) {

            dumpEigFaces( e * 10 + i, x, v[i] );
         }
      }
   }

   ofstream meanout( "meanout.raw" );
   scaleTo255( x, mean );
   meanout.write( x, IMAGE_SIZE );
   meanout.close();
   
   // Euclidean testing
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

   return 0;
}

