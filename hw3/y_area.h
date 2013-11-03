#ifndef Y_AREA_H
#define Y_AREA_H

#include <fstream>

class YArea {
public:
   YArea( unsigned numNeurons, double *x, const unsigned xSize, double *z, const unsigned zSize );
   YArea( std::ifstream* database, double* x, double* z );
   ~YArea();

   double* response;

   void    computePreresponse();
   void    update( bool frozen = false );

   void    writeToDatabase( std::ofstream* database );
   void    saveNeuronBank( const char* fileName, char neuronType, unsigned stride );

private:
   unsigned _numNeurons;

   unsigned _xSize;
   double *_x;
   double **_xNeurons;

   unsigned _zSize;
   double *_z;
   double **_zNeurons;

   // Temporary fields for each preresponse/update
   unsigned _neuronalMatch;
   double*  _sampleX;
   double*  _sampleZ;
   // \Temporary fields

   double  *_neuronalAges;

   double** allocNeuronBank( unsigned neuronSize );
   void     randomizeBank( double **neuronBank, double (*randGen)(), unsigned size );
};

#endif
