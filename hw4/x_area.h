#ifndef X_AREA_H
#define X_AREA_H

class XArea {
public:
   XArea( unsigned responseSize, unsigned numNeurons );
   ~XArea();

   void     setConnections( unsigned* env, double* y );
   unsigned getResponseId();

   // Translations to/from binary representations
   static void     encodeId( unsigned id, double* dst, unsigned dstSize );
   static unsigned decodeId( double* src, unsigned srcSize );

   bool computePreresponse();
   void update();

   double* response;
private:
   unsigned  _responseSize;
   unsigned  _numNeurons;

   // For sampling environment and Y
   unsigned *_env;
   unsigned  _envSample;

   double   *_y;
   double   *_ySample;
   // \sampling

   double  **_neurons;
};

#endif
