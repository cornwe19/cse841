#ifndef Z_AREA_H
#define Z_AREA_H

class ZArea {
public:
   ZArea( unsigned numNeurons ); 
   ~ZArea();
   
   void setY( double* y );

   double* response;

   bool computePreresponse();
   void update();

   void printResponse();

private:
   unsigned _numNeurons;

   double *_y;
   double **_neurons;

   // Temporary fields for each preresponse/update
   unsigned _neuronalMatch;
   double   *_sampleY;
   // \Temporary fields

   double** allocNeuronBank();
};

#endif
