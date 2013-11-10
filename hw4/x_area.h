#ifndef X_AREA_H
#define X_AREA_H

class XArea {
public:
   XArea( unsigned numNeurons );
   ~XArea();

   void     setInputId( unsigned id );
   unsigned getResponseId();

   // Translations to/from binary representations
   static void     encodeId( unsigned id, double* dst, unsigned dstSize );
   static unsigned decodeId( double* src, unsigned srcSize );

   void dumpPredictions();

   double* response;
private:
   unsigned _responseSize;
   unsigned _numNeurons;
   double **_neurons;
   double  *_neuronalAges;

   double _age;
};

#endif
