#include "vectors.h"
#include <cmath>
#include <cfloat>

double Vectors::length( double *vec, unsigned size ) {
   double sum = 0;
   for ( unsigned i = 0; i < size; i++ ) {
      sum += ( vec[i] * vec[i] );
   }

   return sqrt( sum );
}

void Vectors::norm( double *dst, double *src, unsigned size ) {
   double len = length( src, size );

   for ( unsigned i = 0; i < size; i++ ) {
      dst[i] = src[i] / len;
   }
}

void Vectors::scaleTo255( char *dst, double *src, unsigned size ) {
   double max = 0;
   double min = DBL_MAX;

   for ( unsigned i = 0; i < size; i++ ) {
      if ( src[i] > max ) {
         max = src[i];
      }
      if ( src[i] < min ) {
         min = src[i];
      }
   }

   for ( unsigned i = 0; i < size; i++ ) {
      dst[i] = ( 255 * ( (src[i] - min) / (max - min) ) );
   }
}
