#ifndef VECTORS_H
#define VECTORS_H

class Vectors {
public:
   static double length( double *vec, unsigned size );
   static void   norm( double *dst, double *src, unsigned size ); 
   static void   scaleTo255( char *dst, double *src, unsigned size );
};

#endif
