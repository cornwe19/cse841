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
   void update();

   void writeToDatabase( std::ofstream &database );

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
