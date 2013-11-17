#include "neuron_saver.h"
#include "vectors.h"
#include <fstream>

using namespace std;

void NeuronSaver::saveAggregateImage( const char* fileName, double** data, unsigned singleImageSize, unsigned rows, unsigned cols, unsigned stride ) {
   ofstream out( fileName, ios::binary );
   
   unsigned vpad = rows - 1;
   unsigned hpad = cols - 1;
   unsigned totalStride = stride * cols + hpad;
   unsigned imageHeight = singleImageSize / stride;
   unsigned totalHeight = imageHeight * rows + vpad;

   // Write out PGM header
   out << "P5" << endl;
   out << totalStride << endl;
   out << totalHeight << endl;
   out << IMG_WHITE << endl;

   char horizSpacer[totalStride];
   Vectors::fill( horizSpacer, (char)IMG_BLACK, totalStride );

   for ( unsigned row = 0; row < rows; row++ ) {
      char rowImages[rows][singleImageSize];
      for ( unsigned col = 0; col < cols; col++ ) {
         Vectors::scaleTo255( rowImages[col], data[row+col], singleImageSize );
      }

      for ( unsigned line = 0; line < imageHeight; line++ ) {
         for ( unsigned image = 0; image < cols - 1; image++ ) {
            out.write( rowImages[image] + line, stride );
            out.put( IMG_BLACK );
         }
         out.write( rowImages[cols - 1] + line, stride );
      }
      
      if ( row != rows - 1 ) { // Skip trailing spacer
         out.write( horizSpacer, totalStride );
      }
   }

   out.close();
}

void NeuronSaver::saveAges( const char* fileName, double* data, unsigned rows, unsigned cols ) {
   ofstream ages( fileName );

   for ( unsigned row = 0; row < rows; row++ ) {
      for ( unsigned col = 0; col < cols; col++ ) {
         ages << data[row+col] << '\t';
      }
      ages << endl;
   }

   ages.close();
}

