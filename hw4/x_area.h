/**
* Dennis Cornwell
* Implementation of an X area in the developmental network. While supervised, it forwards
* samples from the environment learning the frequency at which one word follows another.
* During thinking it uses those frequencies to predict missing input.
*  - Note that there are 11 neurons to make the math more straightforward. Only neurons 1-10
*    actually get updated with anything since an ID of 0 indicates thinking mode.
**/

#ifndef X_AREA_H
#define X_AREA_H

#include <fstream>

class XArea {
public:
   XArea( unsigned responseSize, unsigned numNeurons );
   XArea( std::ifstream &database );
   ~XArea();

   void     setY( double* y, unsigned ySize );
   void     setY( double* y );
   unsigned getResponseId();

   // Translations to/from binary representations
   static void     encodeId( unsigned id, double* dst, unsigned dstSize );
   static unsigned decodeId( double* src, unsigned srcSize );

   bool computePreresponse( unsigned id );
   void update( bool wasSupervised = true );

   void writeToDatabase( std::ofstream &database );
   void saveNeuronBank( const char* fileName, unsigned stride );
   void saveAges( const char* fileName );

   double* response;
private:
   unsigned  _responseSize;
   unsigned  _numNeurons;

   void init( unsigned responseSize, unsigned numNeurons );

   double   *_y;
   unsigned  _ySize;
   double   *_sampleY;
   double   *_ages;

   unsigned _neuronalMatch;

   double  **_yNeurons;
};

#endif
