#include "vectors.h"
#include <cmath>
#include <cfloat>
#include <stdio.h>

double Vectors::length( double *vec, unsigned size ) {
   double sum = dot( vec, vec, size );

   return sqrt( sum );
}

void Vectors::norm( double *dst, double *src, unsigned size ) {
   double len = length( src, size );

   if ( len > 0 ) {
      for ( unsigned i = 0; i < size; i++ ) {
         dst[i] = src[i] / len;
      }
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

double Vectors::dot( double *a, double *b, unsigned size ) {
   double result = 0;

   for ( unsigned i = 0; i < size; i++ ) {
      result += a[i] * b[i];
   }

   return result;
}

void Vectors::copy( double *dst, double* src, unsigned size ) {
   for ( unsigned i = 0; i < size; i++ ) {
      dst[i] = src[i];
   }
}

void Vectors::print( double* src, unsigned size ) {
   for ( unsigned i = 0; i < size - 1; i++ ) {
      printf( "%.02f, ", src[i] );
   }
   printf( "%.02f", src[size-1] );
}

