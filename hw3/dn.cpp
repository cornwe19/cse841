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
#include "y_area.h"
#include "vectors.h"

#define IMAGE_SIZE 64*88
#define SAMPLE_DURATION 3

using namespace std;

int main( int argc, char** argv ) {
   Settings settings;

   char* error = NULL;
   settings.load( argc, argv, &error );

   if ( error != NULL ) {
       fprintf( stderr, "%s", error );
       exit( 1 );
   }

   ofstream out( settings.outputFile );

   // Training mode
   if ( settings.numEpochs > 0 ) {
      Listing training;
      training.load( settings.listingFile );
    
      out << "Read " << training.size() << " images" << endl;
      out << "Database file '" << settings.networkFile << "'" << endl;

      double  background[IMAGE_SIZE];
      // Init background to 1's
      Vectors::fill( background, 1, IMAGE_SIZE );

      const unsigned zSize = training.numClasses() + 1;

      double  X[IMAGE_SIZE];
      double  Z[zSize];
      YArea   Y( settings.numYNeurons, X, IMAGE_SIZE, Z, zSize );

      Vectors::fill( Z, 0, zSize );

      char currentClass[CLASS_MAX];
      int  classId = 0;

      ofstream database( settings.networkFile );
      unsigned numClasses = training.numClasses();
      database.write( (char*) &numClasses, sizeof(unsigned) );

      int t = 1;
      for ( unsigned image = 0; image < training.size(); image++ ) {
        
         // Skip background images for now
         // Vectors::copy( X, background, IMAGE_SIZE );
         // for ( unsigned bg = 0; bg < SAMPLE_DURATION; bg++ ) {
         //    Y.computePreresponse();
         // }

         Image current( training[image] );
         Vectors::norm( X, current.getData(), IMAGE_SIZE );
         if ( strcmp( current.getClassName(), currentClass ) ) {
            Z[classId] = 0;
            Z[++classId] = 1;
            strcpy( currentClass, current.getClassName() );

            for ( unsigned i = 0; i < zSize; i++ ) {
               printf( "%.0f ", Z[i] );
            }
            printf( "\n" );

            // Write out class mappings as we find them
            database.write( currentClass, strlen( currentClass ) + 1 ); // Write out the null terminator as well
         }

         // Skip presenting same image multiple times for now
         //for ( unsigned sample = 0; sample < SAMPLE_DURATION; sample++ ) {
            Y.computePreresponse();
            Y.update();
         //}
      }

      
      Y.writeToDatabase( &database );

      database.close();

      out.close();
   } 
   // Testing mode
   else {
      Listing testing;
      testing.load( settings.listingFile );

      ifstream database( settings.networkFile );

      unsigned numClasses; 
      database.read( (char*) &numClasses, sizeof(unsigned) );

      char classes[numClasses][CLASS_MAX];
      for ( unsigned i = 0; i < numClasses; i++ ) {
         database.get( classes[i], CLASS_MAX, '\0' );
         database.ignore();
         printf( "Loaded %s\n", classes[i] );
      }

      YArea Y( &database );

      database.close();

      // Do DN testing.
   }

   return 0;
}

