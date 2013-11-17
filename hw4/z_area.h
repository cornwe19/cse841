/*
* Dennis Cornwell
* Z area of the developmental network
*  - Only used in testing while Z is free to read responses from Y
*  - Only finds the top response from Y and reports it back to the caller
*/

#ifndef Z_AREA_H
#define Z_AREA_H

#include <fstream>

class ZArea {
public:
   ZArea( unsigned numNeurons );
   ZArea( std::ifstream &database );
   ~ZArea();
   
   void setY( double* y, unsigned ySize );
   // Attach a Y whose size was loaded via database initialization
   void setY( double* y );

   double* response;

   unsigned getResponseState();

   bool computePreresponse( int trainIndex = - 1 );
   void update( bool isFrozen = false );

   void writeToDatabase( std::ofstream &database );
private:
   unsigned _numNeurons;

   double *_y;
   unsigned _ySize;

   double *_ages;
   double **_yNeurons;

   void init( unsigned numNeurons );

   // Temporary fields for each preresponse/update
   double   *_sampleY;
   unsigned _neuronalMatch;
   // \Temporary fields
};

#endif
