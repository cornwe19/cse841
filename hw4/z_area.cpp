#include "z_area.h"
#include "vectors.h"
#include "settings.h"
#include <stdio.h>
#include <stdlib.h>
#include <cmath>
#include <fstream>

using namespace std;

void ZArea::init( unsigned numNeurons ) {
   _numNeurons = numNeurons;
   
   _ySize = 0;
   _sampleY = NULL;
   _y = NULL;
   _yNeurons = NULL;

   _neuronalMatch = 0;
   _ages = new double[_numNeurons];
   Vectors::fill( _ages, 0.0, _numNeurons );

   response = new double[_numNeurons];
   Vectors::fill( response, 0.0, _numNeurons );
}

ZArea::ZArea( unsigned numNeurons ) {
   init( numNeurons );
}

void ZArea::setY( double* y, unsigned ySize ) {
   _y = y;

   _ySize = ySize;
   _sampleY = new double[_ySize];

   _yNeurons = new double*[_numNeurons];
   for ( unsigned i = 0; i < _numNeurons; i++ ) {
      _yNeurons[i] = new double[_ySize];
      Vectors::fill( _yNeurons[i], 0.0, _ySize );
   }
}

void ZArea::setY( double* y ) {
   _y = y;
}

ZArea::~ZArea() {
   delete [] response;

   if ( _sampleY != NULL ) {
      delete [] _sampleY;
   }

   if ( _yNeurons != NULL ) {
      for ( unsigned i = 0; i < _numNeurons; i++ ) {
         delete [] _yNeurons[i];
      }

      delete [] _yNeurons;
   }
}

bool ZArea::computePreresponse( int trainIndex ) {
   if ( _y == NULL ) {
      return false;
   }

   Vectors::copy( _sampleY, _y, _ySize );

   if ( trainIndex < 0 ) {
      printf( "Testing " );
      Vectors::print( _sampleY, _ySize );
      printf( "\n" );
     
      double normed[_ySize];

      // Search for best fit
      double bestFit = 0;
      for ( unsigned i = 0; i < _numNeurons; i++ ) {
         Vectors::norm( normed, _yNeurons[i], _ySize );
         double fit = Vectors::dot( normed, _sampleY, _ySize );

         if ( fit > bestFit ) {
            bestFit = fit;
            _neuronalMatch = i;
         }
      }

      printf( "Matched " );
      Vectors::print( _yNeurons[_neuronalMatch], _ySize );
      printf( "\n" );
   } else {
      _neuronalMatch = trainIndex;
   }

   return true;
}

unsigned ZArea::getResponseState() {
   return _neuronalMatch;
}

// Update the selected neuron with new frequency weights
void ZArea::update( bool isFrozen ) {
   if ( !isFrozen ) {
      double* firing = _yNeurons[_neuronalMatch];
      double  age    = _ages[_neuronalMatch];
      for ( unsigned i = 0; i < _ySize; i++ ) {
         firing[i] = (age * firing[i]) / (age + 1) + ( ceil(_sampleY[i]) / (age+1) );
      }

      _ages[_neuronalMatch]++;
   }

   Vectors::fill( response, 0.0, _numNeurons );
   response[_neuronalMatch] = 1.0; // Already normalized
}

void ZArea::writeToDatabase( std::ofstream &database ) {
   database.write( (char*) &_numNeurons, sizeof(unsigned) );

   database.write( (char*) &_ySize, sizeof(unsigned) );
   for ( unsigned i = 0; i < _numNeurons; i++ ) {
      database.write( (char*) _yNeurons[i], sizeof(double) * _ySize );
   }
}

ZArea::ZArea( std::ifstream &database ) {
   database.read( (char*) &_numNeurons, sizeof(unsigned) );
   
   init( _numNeurons );
  
   database.read( (char*) &_ySize, sizeof(unsigned) );

   _sampleY = new double[_ySize];

   _yNeurons = new double*[_numNeurons];
   for ( unsigned i = 0; i < _numNeurons; i++ ) {
      _yNeurons[i] = new double[_ySize];
      database.read( (char*) _yNeurons[i], sizeof(double) * _ySize );
   }
}

void ZArea::saveNeuronBank( const char* fileName, unsigned stride ) {
   ofstream out( fileName, ios::binary );
   
   unsigned rows = _numNeurons / 3;
   unsigned cols = _numNeurons / 2;
   unsigned vpad = rows - 1;
   unsigned hpad = cols - 1;
   unsigned totalStride = stride * cols + hpad;
   unsigned imageHeight = _ySize / stride;
   unsigned totalHeight = imageHeight * rows + vpad;

   // Write out PGM header
   out << "P5" << endl;
   out << totalStride << endl;
   out << totalHeight << endl;
   out << IMG_WHITE << endl;

   char horizSpacer[totalStride];
   Vectors::fill( horizSpacer, (char)IMG_BLACK, totalStride );

   for ( unsigned row = 0; row < rows; row++ ) {
      char rowImages[rows][_ySize];
      for ( unsigned col = 0; col < cols; col++ ) {
         Vectors::scaleTo255( rowImages[col], _yNeurons[row+col], _ySize );
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

