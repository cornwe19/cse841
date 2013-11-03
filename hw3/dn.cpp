/**
* Dennis Cornwell
* ICCPCA net implementation with 2 distinct states, 1 for training and the other for testing
*  - Generates a series of MEF eigen vector images, a mean image, a database file and an output report
**/

#include <stdio.h>
#include <fstream>
#include <iomanip>
#include <cmath>
#include <climits>
#include <numeric>
#include <cfloat>
#include <string.h>
#include "settings.h"
#include "listing.h"
#include "image.h"
#include "y_area.h"
#include "z_area.h"
#include "vectors.h"

#define SAMPLE_DURATION 2

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

   // Background image - common to training and testing
   double  background[IMAGE_SIZE];
   Vectors::fill( background, 1.0, IMAGE_SIZE );
   Vectors::norm( background, background, IMAGE_SIZE );


   // Training mode
   if ( settings.numEpochs > 0 ) {
      Listing training;
      training.load( settings.listingFile );
    
      out << "Read " << training.size() << " images" << endl;
      out << "Database file '" << settings.networkFile << "'" << endl;

      const unsigned zSize = training.numClasses() + 1;

      double  X[IMAGE_SIZE];
      double  Z[zSize];
      YArea   Y( settings.numYNeurons, X, IMAGE_SIZE, Z, zSize );

      Vectors::fill( Z, 0.0, zSize );

      char currentClass[CLASS_MAX];
      int  classId = 0;

      ofstream database( settings.networkFile );
      unsigned numClasses = training.numClasses();
      database.write( (char*) &numClasses, sizeof(unsigned) );

      for ( unsigned epoch = 0; epoch < settings.numEpochs; epoch++ ) {
         for ( unsigned image = 0; image < training.size(); image++ ) {

            Image current( training[image] );
            Vectors::norm( X, current.getData(), IMAGE_SIZE );
            // Only write out class mapping on first epoch
            if ( strcmp( current.getClassName(), currentClass ) && epoch == 0 ) {
               Vectors::fill( Z, 0.0, zSize );
               Z[++classId] = 1;
               strcpy( currentClass, current.getClassName() );

               // Write out class mappings as we find them
               // (Write out the null terminator as well)
               database.write( currentClass, strlen( currentClass ) + 1 ); 
            }
           
            for ( unsigned d = 0; d < SAMPLE_DURATION; d++ ) {
               Y.computePreresponse();
               Y.update();
            }
         }
      }
      
      Y.saveNeuronBank( "stem-xy.pgm", 'X', IMAGE_STRIDE );
      Y.saveNeuronBank( "stem-yz.pgm", 'Z', 5 );
      Y.saveAges( "stem-ay.txt" );

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
      }

      double X[IMAGE_SIZE];
      ZArea  Z( numClasses + 1 );
      YArea  Y( &database, X, Z.response );
      Z.setY( Y.response );

      database.close();

      int numCorrect = 0;

      ofstream output( settings.outputFile );

      // Init X with a background Image
      Vectors::copy( X, background, IMAGE_SIZE );
      for ( unsigned image = 0; image < testing.size(); image++ ) {
         
         Image current( testing[image] );

         // Find the index of the image's class
         unsigned currentClass = 0;
         for ( unsigned i = 0; i < numClasses; i++ ) {
            if ( !strcmp( current.getClassName(), classes[i] ) ) {
               currentClass = i;
               break;
            }
         }

         // Compute preresponses (note that X doesn't have one)
         Y.computePreresponse();
         if ( !Z.computePreresponse() ) {
            fprintf( stderr, "Forgot to hook up _y area\n" );
            exit( 1 );
         }

         // Do updates
         Vectors::norm( X, current.getData(), IMAGE_SIZE );
         Y.update( true ); // Y is frozen
         unsigned selectedClass = Z.update();

         output << setw( 12 ) << current.getClassName() << setw( 12 ) << classes[selectedClass] << endl;
         
         if ( selectedClass == currentClass ) {
            numCorrect++;
         }
      }

      output << "Matched " << setprecision( 3 ) <<  (double)numCorrect / (double)testing.size() * 100 << "% of images correctly" << endl;

      output.close();
   }

   return 0;
}

