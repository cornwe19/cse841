/**
* Dennis Cornwell
* ICCPCA net implementation with 2 distinct states, 1 for training and the other for testing
*  - Generates a series of MEF eigen vector images, a mean image, a database file and an output report
**/

#include <stdio.h>
#include <fstream>
#include <cmath>
#include <climits>
#include <numeric>
#include <cfloat>
#include <string.h>
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

void scaleTo255( char* dest, double* src, unsigned size ) {
   double max = 0;
   double min = DBL_MAX;

   for ( unsigned i = 0; i < size; i++ ) {
      if ( src[i] > max ) {
         max = src[i];
      }
      if ( src[i] < min ) {
         min = src[i];
      }
   }

   for ( unsigned i = 0; i < size; i++ ) {
      dest[i] = ( 255 * ( (src[i] - min) / (max - min) ) );
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

   // Ran out of time to calculate represented variance
   const unsigned k = 100; 

   ofstream out( settings.outputFile );

   // Training mode
   if ( settings.numEpochs > 0 ) {
      Listing training;
      training.load( settings.listingFile );
    
      out << "Read " << training.size() << " images" << endl;
      out << "Database file '" << settings.networkFile << "'" << endl;
      out << "Using " << k << " MEFs. Unknown represented variance" << endl;

      // Mean image
      double mean[IMAGE_SIZE];
      double u   [IMAGE_SIZE];
      char   x   [IMAGE_SIZE]; // Scratch buffer for conversion from double*
      
      int n = 2;
      double w1 = 0;
      double w2 = 0;

      vector<double*> v;

      for ( int e = 0; e < settings.numEpochs; e++ ) {
         for ( unsigned i = 0; i < training.size(); i++ ) {

            if ( e == 0 && i == 0 ) {
               Image init( training[0] );
               for ( int p = 0; p < IMAGE_SIZE; p++ ) {
                  mean[p] = init[p];
               }

               continue;
            }

            Image img( training[i] );

            double tOverN = ( (double)( n ) / (double)( n + 1 ) );
            double oneOverN = ( 1.0f / (double)( n + 1 ) );
            for ( int p = 0; p < IMAGE_SIZE; p++ ) {
               mean[p] = tOverN * mean[p] + oneOverN * img[p];
               u[p] = img[p] - mean[p];
            }
            
            amnesic( n++, &w1, &w2 );
            
            for ( unsigned comp = 0; comp < min( i, k ); comp++ ) {

               if ( v.size() == comp ) {
                  double* vi = new double[IMAGE_SIZE];
                  for ( int j = 0; j < IMAGE_SIZE; j++ ) {
                     vi[j] = u[j];
                  }

                  v.push_back( vi );
               } else {
                  
                  double lvi = vecLen( v[comp], IMAGE_SIZE );
                  
                  double yi = 0;
                  for ( int j = 0; j < IMAGE_SIZE; j++ ) {
                     yi += u[j] * v[comp][j] / lvi;
                  }

                  for ( int j = 0; j < IMAGE_SIZE; j++ ) {
                     // Update current principal component estimate
                     v[comp][j] = w1 * v[comp][j] + w2 * yi * u[j];
                  }

                  lvi = vecLen( v[comp], IMAGE_SIZE ); // Update norm(v[i])

                  for ( int j = 0; j < IMAGE_SIZE; j++ ) {
                     // Update residual for next component
                     u[j] = u[j] - yi * v[comp][j] / lvi;
                  }
               }
            }
         }
      }

      ofstream db( settings.networkFile, ios::binary );
      scaleTo255( x, mean, IMAGE_SIZE );
      db.write( x, IMAGE_SIZE );

      db.put( '\n' );

      ofstream meanout( "out/meanout.raw" );
      meanout.write( x, IMAGE_SIZE );
      meanout.close();

      out << "Mean filename: out/meanout.raw" << endl;

      for ( unsigned j = 0; j < v.size(); j++ ) {
         scaleTo255( x, v[j], IMAGE_SIZE );

         db.write( x, IMAGE_SIZE );

         char outName[128];
         sprintf( outName, "out/mef-%d.raw", j );

         out << "MEF " << j << " file: " << outName << endl;

         ofstream viOut( outName, ios::binary );
         viOut.write( x, IMAGE_SIZE );
         viOut.close();
      }

      db.put( '\n' );

      for ( unsigned j = 0; j < training.size(); j++ ) {
         Image train( training[j] );
         double proj[v.size()];
         
         for ( unsigned p = 0; p < v.size(); p++ ) {
            proj[p] = 0;
            for ( unsigned i = 0; i < IMAGE_SIZE; i++ ) {
                proj[p] += ( train[i] - mean[i] ) * v[p][i];
            }
         }

         scaleTo255( x, proj, v.size() );

         db.write( train.getClassName(), strlen( train.getClassName() ) );
         db.put( '\n' );
         db.write( x, v.size() );
      }

      db.close();
      out.close();
   } 
   // Testing mode
   else {
      Listing testing;
      testing.load( settings.listingFile );

      char mean[IMAGE_SIZE];
      char x[IMAGE_SIZE]; // scratch buffer

      for ( unsigned i = 0; i < testing.size(); i++ ) {
         ifstream db( settings.networkFile );
         db.read( mean, IMAGE_SIZE );

         db.ignore( 1 ); // skip line break

         vector<double*> v;
         for ( unsigned e = 0; e < k; e++ ) {
            double vi[IMAGE_SIZE];

            db.read( x, IMAGE_SIZE );
            for ( int j = 0; j < IMAGE_SIZE; j++ ) {
               vi[j] = (double)x[j];
            }

            v.push_back( vi );
         }
         
         db.ignore( 1 ); // skip line break

         Image test( testing[i] );
         double leastDistance = DBL_MAX;
         char matchedClass[CLASS_MAX];

         double u[v.size()];
         for ( unsigned j = 0; j < v.size(); j++ ) {
            u[j] = 0;
            for ( int p = 0; p < IMAGE_SIZE; p++ ) {
               u[j] += ( test[p] - (double)mean[p] ) * v[j][p];
            }
         }

         char className[CLASS_MAX];
         while ( db >> className ) {
            db.ignore( 1 ); // skip line break

            db.read( x, k );

            // Compute euclidean dist between scatter and test vector
            double dist = 0;
            for ( unsigned j = 0; j < v.size(); j++ ) {
               dist += pow( (double)x[j] - u[j], 2 );
            }
            dist = sqrt( dist );
            
            if ( dist < leastDistance ) {
               leastDistance = dist;
               
               strcpy( matchedClass, className );
            }
         }

         out << "Matched " << test.getFileName() << " to " << className << " with distance " << leastDistance << endl;

         db.close();
      }
   }

   return 0;
}

