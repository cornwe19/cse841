#ifndef Z_AREA_H
#define Z_AREA_H

class ZArea {
public:
   ZArea( unsigned numNeurons ); 
   ~ZArea();
   
   void setY( double* y );

   double* response;

   bool computePreresponse();
   void update( double &strongestResponse, unsigned &responseIndex );

private:
   unsigned _numNeurons;

   double *_y;

   // Temporary fields for each preresponse/update
   double   *_sampleY;
   // \Temporary fields
};

#endif
