#ifndef AREA_H
#define AREA_H

class Area {
public:
   Area( unsigned nuerons, Area *b, Area *t );
   ~Area();

   double response( unsigned index ) { return _response[index]; }
   double* response() { return _response; }
   unsigned numNeurons() { return _numNeurons; }

private:
   Area *_b;
   double *_synWeightB;

   Area *_t;
   double *_synWeightT;
   
   unsigned _numNeurons;
   unsigned *_ages;
   double *_response;
};

#endif
