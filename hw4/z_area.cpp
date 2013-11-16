#include "z_area.h"
#include "vectors.h"
#include <stdio.h>
#include <stdlib.h>
#include <cmath>
#include <fstream>

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

   printf( "Z looks like:\n" );
   for ( unsigned i = 0; i < _numNeurons; i++ ) {
      Vectors::print( _yNeurons[i], _ySize );
      printf( "\n" );
   }
}

