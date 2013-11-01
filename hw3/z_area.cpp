#include "z_area.h"
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
