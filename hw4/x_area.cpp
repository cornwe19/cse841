#include "x_area.h"

XArea::XArea( unsigned numNeurons ) {
   _numNeurons = numNeurons;

   response = new double[numNeurons];
}

XArea::~XArea() {
   delete [] response;
}

void XArea::setInputId( unsigned id ) {
   encodeId( id, response, _numNeurons );
}

unsigned XArea::getResponseId() {
   return decodeId( response, _numNeurons );
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

