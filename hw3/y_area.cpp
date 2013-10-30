#include "y_area.h"

YArea::YArea( unsigned numNeurons, double *x, const unsigned xSize, double *z, const unsigned zSize ) {
   _numNeurons = numNeurons;
   _xSize = xSize;
   _zSize = zSize;

   _x = x;
   _z = z;

   _xNeurons = allocNeuronBank( xSize );
   _zNeurons = allocNeuronBank( zSize );

   _response = new double[zSize];
}

YArea::~YArea() {
   for ( unsigned i = 0; i < _numNeurons; i++ ) {
      delete [] _xNeurons[i];
      delete [] _zNeurons[i];
   }

   delete [] _xNeurons;
   delete [] _zNeurons;

   delete [] _response;
}

double** YArea::allocNeuronBank( unsigned neuronSize ) {
   double **bank = new double*[_numNeurons];

   for ( unsigned i = 0; i < _numNeurons; i++ ) {
      bank[i] = new double[neuronSize];
   }

   return bank;
}

void YArea::computePreresponse() {
   // Find matching neuron and update it 
}

void YArea::update() {
   // Copy new values from matching neuron into _response
}
