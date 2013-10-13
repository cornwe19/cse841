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

int main( int argc, char** argv ) {
   Settings settings;

   char* error = NULL;
   settings.load( argc, argv, &error );

   if ( error != NULL ) {
       fprintf( stderr, "%s", error );
       exit( 1 );
   }

   // Training mode
   if ( settings.numEpochs > 0 ) {
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

         bool endOfTraining =  e == settings.numEpochs - 1;
         if ( endOfTraining ) {
            ofstream db( settings.networkFile, ios::binary );
            scaleTo255( x, mean );
            db.write( x, IMAGE_SIZE );

            db.put( '\n' );

            ofstream meanout( "out/meanout.raw" );
            scaleTo255( x, mean );
            meanout.write( x, IMAGE_SIZE );
            meanout.close();

            for ( unsigned i = 0; i < training.numClasses(); i++ ) {
               for ( unsigned j = 0; j < v[i].size(); j++ ) {
                  scaleTo255( x, v[i][j] );

                  char* className = training.classAt( i )->getName();
                  db.write( className, strlen( className ) );
                  db.put( '\n' );
                  db.write( x, IMAGE_SIZE );

                  char outName[128];
                  sprintf( outName, "out/%s-%d.raw", className, i );
                  ofstream viOut( outName, ios::binary );
                  viOut.write( x, IMAGE_SIZE );
                  viOut.close();
               }
            }
         }
      }
   } 
   // Testing mode
   else {
      Listing testing;
      testing.load( settings.listingFile );

      int numRight = 0;
      int total = 0;

      char mean[IMAGE_SIZE];
      double u[IMAGE_SIZE];
      char t[IMAGE_SIZE];

      for ( unsigned i = 0; i < testing.size(); i++ ) {
         Image test( testing[i] );
         double leastDistance = DBL_MAX;
         char matchedClass[CLASS_MAX];

         ifstream db( settings.networkFile );
         db.read( mean, IMAGE_SIZE );

         for ( unsigned j = 0; j < IMAGE_SIZE; j++ ) {
            u[j] = test[j] - (double)mean[j];
         }

         db.ignore( 1 ); // skip line break

         char className[CLASS_MAX];
         while ( db >> className ) {
            db.ignore( 1 ); // skip line break

            db.read( t, IMAGE_SIZE );

            // Compute euclidean dist between scatter and test vector
            double dist = 0;
            for ( unsigned j = 0; j < IMAGE_SIZE; j++ ) {
               dist += pow( (double)t[j] - u[j], 2 );
            }
            dist = sqrt( dist );
            
            if ( dist < leastDistance ) {
               leastDistance = dist;
               
               strcpy( matchedClass, className );
            }
         }

         if ( !strcmp( test.getClassName(), matchedClass ) ) {
            numRight++;
         }
         total++;

         printf( "%s matched %s\n", test.getClassName(), matchedClass );
      }

      printf( "Classifier was right %.2f of the time\n", (float)numRight / (float)total );
   }

   return 0;
}

