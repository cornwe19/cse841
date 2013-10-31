#ifndef Y_AREA_H
#define Y_AREA_H

class YArea {
public:
   YArea( unsigned numNeurons, double *x, const unsigned xSize, double *z, const unsigned zSize );
   ~YArea();

   double* response() { return _response; }
   void    computePreresponse();
   void    update();

private:
   unsigned _numNeurons;

   unsigned _xSize;
   double *_x;
   double **_xNeurons;

   unsigned _zSize;
   double *_z;
   double **_zNeurons;

   // Pointer to most recent preresponse
   // to be copied into response during an update
   unsigned _neuronalMatch;
   double *_response;

   double** allocNeuronBank( unsigned neuronSize );
   void     randomizeBank( double **neuronBank, unsigned size );
};

#endif
