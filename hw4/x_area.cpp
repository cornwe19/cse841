#include "x_area.h"
#include "vectors.h"
#include <cstdlib>
#include <cstdio>
#include <cmath>
#include <fstream>

void XArea::init( unsigned responseSize, unsigned numNeurons ) {
   _responseSize = responseSize;
   _numNeurons = numNeurons;

   response = new double[responseSize];
   Vectors::fill( response, 0.0, _responseSize );

   _neuronalMatch = 0;

   _yNeurons = NULL;
   _ySize = 0;
   _sampleY = NULL;

   _ages = new double[numNeurons];
   Vectors::fill( _ages, 0.0, numNeurons );
}

XArea::XArea( unsigned responseSize, unsigned numNeurons ) {
   init( responseSize, numNeurons );
}

XArea::XArea( std::ifstream &database ) {
   database.read( (char*)&_responseSize, sizeof(unsigned) );
   database.read( (char*)&_numNeurons, sizeof(unsigned) );

   init( _responseSize, _numNeurons );

   database.read( (char*)&_ySize, sizeof(unsigned) );
   _sampleY = new double[_ySize];

   _yNeurons = new double*[_numNeurons];
   printf( "X looks like:\n" );
   for ( unsigned i = 0; i < _numNeurons; i++ ) {
      _yNeurons[i] = new double[_ySize];
      database.read( (char*)_yNeurons[i], sizeof(double) * _ySize );
      Vectors::print( _yNeurons[i], _ySize );
      printf( "\n" );
   }
}

XArea::~XArea() {
   delete [] response;

   if ( _yNeurons != NULL ) {
      for ( unsigned i = 0; i < _numNeurons; i++ ) {
         delete [] _yNeurons[i];
      }

      delete [] _yNeurons;
      delete [] _sampleY;
   }
}

unsigned XArea::getResponseId() {
   return decodeId( response, _responseSize );
}

// Gets the binary encoded ID normalized
void XArea::encodeId( unsigned id, double* dst, unsigned dstSize ) {
   for ( unsigned i = 0; i < dstSize; i++ ) {
      dst[i] = ( id >> ( dstSize - i - 1 ) ) & 1;
   }

   Vectors::norm( dst, dst, dstSize );
}

// Convert from an optionally normalized binary representation of X to an ID
unsigned XArea::decodeId( double* src, unsigned srcSize ) {
   unsigned id = 0;

   for ( unsigned i = 0; i < srcSize; i++ ) {
      id <<= 1;
      id += ( ( (unsigned)ceil(src[i]) ) & 1 );
   }

   return id;
}

void XArea::setY( double* y, unsigned ySize ) {
   _y = y;
   _ySize = ySize;

   _sampleY = new double[_ySize];

   _yNeurons = new double*[_numNeurons];
   for ( unsigned i = 0; i < _numNeurons; i++ ) {
      _yNeurons[i] = new double[_ySize];
      Vectors::fill( _yNeurons[i], 0.0, _ySize );
   }
}

void XArea::setY( double* y ) {
   _y = y;
}

bool XArea::computePreresponse( unsigned id ) {
   if ( _y == NULL ) {
      return false;
   }

   Vectors::copy( _sampleY, _y, _ySize );

   if ( id > 0  ) {
      _neuronalMatch = id; 
   } else {
      // Thinking mode. Estimate next X
      double bestFit = 0;
      double normed[_ySize];
      for ( unsigned i = 0; i < _numNeurons; i++ ) {
         Vectors::norm( normed, _yNeurons[i], _ySize );
         double fit = Vectors::dot( normed, _sampleY, _ySize );

         if ( fit > bestFit ) {
            bestFit = fit;
            _neuronalMatch = i;
         }
      }
   }

   // Propogate current X immediately
   encodeId( _neuronalMatch, response, _responseSize );

   return true; 
}

void XArea::update() {
   double* firing = _yNeurons[_neuronalMatch];
   double  age    = _ages[_neuronalMatch];
   for ( unsigned i = 0; i < _ySize; i++ ) {
      firing[i] = (age * firing[i]) / (age + 1) + ( ceil(_sampleY[i]) / (age+1) );
   }

   _ages[_neuronalMatch]++;
}

void XArea::writeToDatabase( std::ofstream &database ) {
   database.write( (char*)&_responseSize, sizeof(unsigned) );
   database.write( (char*)&_numNeurons, sizeof(unsigned) );

   database.write( (char*)&_ySize, sizeof(unsigned) );
   
   for ( unsigned i = 0; i < _numNeurons; i++ ) {
      printf( "Writing " );
      Vectors::print( _yNeurons[i], _ySize );
      printf( "\n" );
      database.write( (char*)_yNeurons[i], sizeof(double) * _ySize );
   }
}
