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
   double localX[_xSize], sampleX[_xSize];
   double localZ[_zSize], sampleZ[_zSize];

   for ( unsigned i = 0; i < _numNeurons; i++ ) {
      Vectors::norm( localX, _x, _xSize );
      Vectors::norm( sampleX, _xNeurons[i], _xSize );
      double xFit = Vectors::dot( localX, sampleX, _xSize );

      Vectors::norm( localZ, _z, _zSize );
      Vectors::norm( sampleZ, _zNeurons[i], _zSize );
      double zFit = Vectors::dot( localZ, sampleZ, _zSize );

      if ( xFit + zFit > bestFit ) {
         bestFit = xFit + zFit;
         _neuronalMatch = i;
      }
   }

   printf( "Found best match %d(%f)\n", _neuronalMatch, bestFit );
}

void YArea::update() {
   // Copy new values from matching neuron into _response
}
