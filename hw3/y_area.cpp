#include "y_area.h"
#include "vectors.h"
#include <stdlib.h>
#include <stdio.h>
#include <time.h>

YArea::YArea( unsigned numNeurons, double *x, const unsigned xSize, double *z, const unsigned zSize ) {
   _numNeurons = numNeurons;
   _xSize = xSize;
   _zSize = zSize;

   _x = x;
   _z = z;

   _xNeurons = allocNeuronBank( xSize );
   _zNeurons = allocNeuronBank( zSize );

   srand( time( NULL ) );
   randomizeBank( _xNeurons, _xSize );
   randomizeBank( _zNeurons, _zSize );

   _neuronalAges = new double[numNeurons];
   Vectors::fill( _neuronalAges, 1, numNeurons );

   _sampleX = new double[_xSize];
   _sampleZ = new double[_zSize];
}

YArea::~YArea() {
   for ( unsigned i = 0; i < _numNeurons; i++ ) {
      delete [] _xNeurons[i];
      delete [] _zNeurons[i];
   }

   delete [] _xNeurons;
   delete [] _zNeurons;

   delete [] _neuronalAges;
   
   delete [] _sampleX;
   delete [] _sampleZ;
}

double** YArea::allocNeuronBank( unsigned neuronSize ) {
   double **bank = new double*[_numNeurons];

   for ( unsigned i = 0; i < _numNeurons; i++ ) {
      bank[i] = new double[neuronSize];
   }

   return bank;
}

void YArea::randomizeBank( double **neuronBank, unsigned size ) {
   for ( unsigned i = 0; i < _numNeurons; i++ ) {
      for ( unsigned j = 0; j < size; j++ ) {
         neuronBank[i][j] = ( rand() % 255 );
      }
   }

   for ( unsigned i = 0; i < _numNeurons; i++ ) {
      Vectors::norm( neuronBank[i], neuronBank[i], size );
   }
}

void YArea::computePreresponse() {
   // Find matching neuron and update it 
   double bestFit = 0;

   for ( unsigned i = 0; i < _numNeurons; i++ ) {
      Vectors::copy( _sampleX, _x, _xSize );
      Vectors::copy( _sampleZ, _z, _zSize );

      double xFit = Vectors::dot( _xNeurons[i], _sampleX, _xSize );
      double zFit = Vectors::dot( _zNeurons[i], _sampleZ, _zSize );

      if ( xFit + zFit > bestFit ) {
         bestFit = xFit + zFit;
         _neuronalMatch = i;
      }
   }

   printf( "Found best match %d (%f)\n", _neuronalMatch, bestFit );
}

void YArea::update() {
   // Update weight vectors for matched neuron
   double age = _neuronalAges[_neuronalMatch]++;
   double w1 = ( age - 1.0 ) / 1.0;
   double w2 = 1.0 / age;

   double* xWeights = _xNeurons[_neuronalMatch];
   double* zWeights = _zNeurons[_neuronalMatch];

   for ( unsigned i = 0; i < _xSize; i++ ) {
      xWeights[i] = w1 * xWeights[i] + w2 * _sampleX[i];
   }

   for ( unsigned i = 0; i < _zSize; i++ ) {
      zWeights[i] = w1 * zWeights[i] + w2 * _sampleZ[i];
   }

   // Re-normalize weights once we've updated
   Vectors::norm( xWeights, xWeights, _xSize );
   Vectors::norm( zWeights, zWeights, _zSize );
}
