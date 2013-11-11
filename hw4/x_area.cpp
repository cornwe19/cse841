#include "x_area.h"
#include "vectors.h"
#include <cstdlib>
#include <cstdio>

XArea::XArea( unsigned responseSize, unsigned numNeurons ) {
   _responseSize = responseSize;
   _numNeurons = numNeurons;

   response = new double[responseSize];

   _neurons = new double*[_numNeurons];
   for ( unsigned i = 0; i < _numNeurons; i++ ) {
      _neurons[i] = new double[_responseSize];
      Vectors::fill( _neurons[i], 0.0, _responseSize );
   }

   _ySample = new double[responseSize];
}

XArea::~XArea() {
   delete [] response;

   for ( unsigned i = 0; i < _numNeurons; i++ ) {
      delete [] _neurons[i];
   }

   delete [] _neurons;

   delete [] _ySample;
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

void XArea::setConnections( unsigned* env, double* y ) {
   _y = y;
   _env = env;
}

bool XArea::computePreresponse() {
   if ( _y == NULL ) {
      return false;
   } 

   Vectors::copy( _ySample, _y, _responseSize );
   _envSample = *_env;

   return true; 
}

void XArea::update() {
   encodeId( _envSample, response, _responseSize ); 
}
