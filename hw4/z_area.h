/*
* Dennis Cornwell
* Z area of the developmental network
*  - Only used in testing while Z is free to read responses from Y
*  - Only finds the top response from Y and reports it back to the caller
*/

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
