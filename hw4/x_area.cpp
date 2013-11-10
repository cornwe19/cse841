#include <cmath>
#include <cstdio>
#include <algorithm>
#include "vectors.h"
#include "x_area.h"

XArea::XArea( unsigned responseSize ) {
   _responseSize = responseSize;
   _numNeurons   = (unsigned)pow( 2, responseSize );

   response = new double[_numNeurons];
   _neurons = new double*[_numNeurons];

   for ( unsigned i = 0; i < _numNeurons; i++ ) {
      _neurons[i] = new double[responseSize];
      Vectors::fill( _neurons[i], 0.0, responseSize );
   }

   _age = 0;

   _neuronalAges = new double[_numNeurons];
   Vectors::fill( _neuronalAges, 1.0, _numNeurons );
}

XArea::~XArea() {
   delete [] response;

   for ( unsigned i = 0; i < _numNeurons; i++ ) {
      delete [] _neurons[i];
   }

   delete [] _neurons;
}

void XArea::setInputId( unsigned id ) {
   int lastIndex = (int)getResponseId() - 1;

   encodeId( id, response, _responseSize );

   // Update transition prediction
   if ( lastIndex >= 0 ) {
      _neuronalAges[lastIndex]++;
      for ( unsigned i = 0; i < _responseSize; i++ ) {
         _neurons[lastIndex][i] = ( response[i] + _neurons[lastIndex][i] ) / _neuronalAges[lastIndex]; 
      }
   }
}

unsigned XArea::getResponseId() {
   return decodeId( response, _responseSize );
}

void XArea::encodeId( unsigned id, double* dst, unsigned dstSize ) {
   for ( unsigned i = 0; i < dstSize; i++ ) {
      dst[i] = ( id >> ( dstSize - i - 1 ) ) & 1;
   }
}

unsigned XArea::decodeId( double* src, unsigned srcSize ) {
   unsigned id = 0;

   for ( unsigned i = 0; i < srcSize; i++ ) {
      id <<= 1;
      id += ( ( (unsigned)src[i] ) & 1 );
   }

   return id;
}

void XArea::dumpPredictions() { 
   for ( unsigned i = 0; i < _responseSize; i++ ) {
      for ( unsigned j = 0; j < _numNeurons; j++ ) {
         printf( " %0.07f ", _neurons[j][i] );
      }
      printf( "\n" );
   }
}

