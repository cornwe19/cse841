#include "z_area.h"
#include "vectors.h"
#include <stdio.h>
#include <stdlib.h>
#include <fstream>

void ZArea::init( unsigned numNeurons ) {
   _numNeurons = numNeurons;
   
   _ySize = 0;
   _sampleY = NULL;
   _y = NULL;
   _yNeurons = NULL;

   _neuronalMatch = 0;
   _age = 0;

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
      // Search for best fit
      double bestFit = 0;
      for ( unsigned i = 0; i < _numNeurons; i++ ) {
         double fit = Vectors::dot( _yNeurons[i], _sampleY, _ySize );

         if ( fit > bestFit ) {
            bestFit = fit;
            _neuronalMatch = i;
         }
      }
   } else {
      _neuronalMatch = trainIndex;
   }

   return true;
}

// Update the selected neuron with new frequency weights
void ZArea::update() {
   _age++;
   for ( unsigned neuron = 0; neuron < _numNeurons; neuron++ ) {
      for ( unsigned i = 0; i < _ySize; i++ ) {
         if ( neuron == _neuronalMatch ) {
            _yNeurons[neuron][i] += _sampleY[i];
         }

          _yNeurons[neuron][i] = _yNeurons[neuron][i] / _age;
      }

      Vectors::norm( _yNeurons[neuron], _yNeurons[neuron], _ySize );
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

