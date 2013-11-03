#ifndef VECTORS_H
#define VECTORS_H

class Vectors {
public:
   static double length( double *vec, unsigned size );
   static void   norm( double *dst, double *src, unsigned size );
   static double dot( double *a, double *b, unsigned size );
   static void   scaleTo255( char *dst, double *src, unsigned size );
   static void   copy( double* dst, double* src, unsigned size );
   template <class T>
   static void   fill( T* dst, T value, unsigned size ) {
      for ( unsigned i = 0; i < size; i++ ) {
         dst[i] = value;
      }
   }
};

#endif
