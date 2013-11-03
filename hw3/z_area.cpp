#include "z_area.h"
#include "vectors.h"
#include <stdio.h>
#include <stdlib.h>

ZArea::ZArea( unsigned numNeurons ) {
   _numNeurons = numNeurons;
   
   _sampleY = new double[_numNeurons];
   _y = NULL;

   response = new double[_numNeurons];
   Vectors::fill( response, 0.0, _numNeurons );
}

void ZArea::setY( double* y ) {
   _y = y;
}

ZArea::~ZArea() {
   delete [] response;

   delete [] _sampleY;
}

bool ZArea::computePreresponse() {
   if ( _y == NULL ) {
      return false;
   }

   Vectors::copy( _sampleY, _y, _numNeurons );

   return true;
}

// Since Z doesn't really 'learn', just store out the strongest response
void ZArea::update( double &strongestResponse, unsigned &responseIndex ) {
   strongestResponse = _sampleY[0];
   responseIndex     = 0;

   for ( unsigned i = 1; i < _numNeurons; i++ ) {
      if ( _sampleY[i] > strongestResponse ) {
         strongestResponse = _sampleY[i];
         responseIndex = i;
      }
   }
}

