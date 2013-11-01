#include "z_area.h"
#include "vectors.h"
#include <stdio.h>
#include <stdlib.h>

ZArea::ZArea( unsigned numNeurons ) {
   _numNeurons = numNeurons;
   
   _sampleY = new double[_numNeurons];
   _y = NULL;

   _neurons = allocNeuronBank();

   response = _neurons[0];
}

void ZArea::setY( double* y ) {
   _y = y;
}

ZArea::~ZArea() {
   for ( unsigned i = 0; i < _numNeurons; i++ ) {
      delete [] _neurons[i];
   }

   delete [] _neurons;

   delete [] _sampleY;
}

double** ZArea::allocNeuronBank() {
   double** Z = new double*[_numNeurons];

   for ( unsigned i = 0; i < _numNeurons; i++ ) {
      Z[i] = new double[_numNeurons];
      for ( unsigned j = 0; j < _numNeurons; j++ ) {
         if ( i == j ) {
            Z[i][j] = 1;
         } else {
            Z[i][j] = 0;
         }
      }
   }

   return Z;
}

bool ZArea::computePreresponse() {
   if ( _y == NULL ) {
      return false;
   }
   
   double bestFit = 0;

   for ( unsigned i = 0; i < _numNeurons; i++ ) {
      Vectors::copy( _sampleY, _y, _numNeurons );
      
      double yFit = Vectors::dot( _neurons[i], _sampleY, _numNeurons );

      if ( yFit > bestFit ) {
         bestFit = yFit;
         _neuronalMatch = i;
      }
   }

   printf( "Z: Found best match %d (%f)\n", _neuronalMatch, bestFit );

   return true;
}

// Since Z doesn't really 'learn', just store out the neuron we found
void ZArea::update() {
   response = _neurons[_neuronalMatch];
}

void ZArea::printResponse() {
   for ( unsigned i = 0; i < _numNeurons; i++ ) {
      printf( "%.0f ", response[i] );
   }
   printf( "\n" );
}
