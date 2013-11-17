/*
* Dennis Cornwell
* Z area of the developmental network. Uses frequency of state transitions to predict
* future transitions in testing. During training, builds frequencies based on trained
* neuron selection.
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
   void saveNeuronBank( const char* fileName, unsigned stride );
   void saveAges( const char* fileName );
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
