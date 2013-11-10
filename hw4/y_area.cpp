#include "y_area.h"
#include "vectors.h"
#include <stdlib.h>
#include <stdio.h>
#include <time.h>
#include <fstream>
#include <cmath>

#define WHITE 255

using namespace std;

double  classDataGenerator() { return ( (double)rand() / RAND_MAX ); }

YArea::YArea( unsigned numNeurons, double *x, const unsigned xSize, double *z, const unsigned zSize ) {
   _numNeurons = numNeurons;
   _xSize = xSize;
   _zSize = zSize;

   _x = x;
   _z = z;

   _xNeurons = allocNeuronBank( xSize );
   _zNeurons = allocNeuronBank( zSize );

   srand( time( NULL ) );
   randomizeBank( _xNeurons, classDataGenerator, _xSize );
   randomizeBank( _zNeurons, classDataGenerator, _zSize );
   
   response = new double[_zSize];

   _neuronalAges = new double[numNeurons];
   Vectors::fill( _neuronalAges, 1.0, numNeurons );

   _sampleX = new double[_xSize];
   _sampleZ = new double[_zSize];
}

YArea::~YArea() {
   for ( unsigned i = 0; i < _numNeurons; i++ ) {
      delete [] _xNeurons[i];
      delete [] _zNeurons[i];
   }

   delete [] _xNeurons;
   delete [] _zNeurons;

   delete [] _neuronalAges;
   
   delete [] _sampleX;
   delete [] _sampleZ;

   delete [] response;
}

double** YArea::allocNeuronBank( unsigned neuronSize ) {
   double **bank = new double*[_numNeurons];

   for ( unsigned i = 0; i < _numNeurons; i++ ) {
      bank[i] = new double[neuronSize];
   }

   return bank;
}

void YArea::randomizeBank( double **neuronBank, double (*randGen)(), unsigned size ) {
   for ( unsigned i = 0; i < _numNeurons; i++ ) {
      for ( unsigned j = 0; j < size; j++ ) {
         neuronBank[i][j] = (*randGen)();
      }
   }

   for ( unsigned i = 0; i < _numNeurons; i++ ) {
      Vectors::norm( neuronBank[i], neuronBank[i], size );
   }
}

void YArea::computePreresponse() {
   // Find matching neuron and update it 
   double bestFit = 0;

   Vectors::copy( _sampleX, _x, _xSize );
   Vectors::copy( _sampleZ, _z, _zSize );
   
   for ( unsigned i = 0; i < _numNeurons; i++ ) {
      double xFit = Vectors::dot( _xNeurons[i], _sampleX, _xSize );
      double zFit = Vectors::dot( _zNeurons[i], _sampleZ, _zSize );

      if ( xFit + zFit > bestFit ) {
         bestFit = xFit + zFit;
         _neuronalMatch = i;
      }
   }
}

void YArea::update( bool frozen ) {
   double* zWeights = _zNeurons[_neuronalMatch];
   
   if ( !frozen ) {
      // Update weight vectors for matched neuron
      double age = ++_neuronalAges[_neuronalMatch];
      double w1 = ( age - 1.0 ) / 1.0;
      double w2 = 1.0 / age;

      double* xWeights = _xNeurons[_neuronalMatch];

      for ( unsigned i = 0; i < _xSize; i++ ) {
         xWeights[i] = w1 * xWeights[i] + w2 * _sampleX[i];
      }

      for ( unsigned i = 0; i < _zSize; i++ ) {
         zWeights[i] = w1 * zWeights[i] + w2 * _sampleZ[i];
      }

      // Re-normalize weights once we've updated
      Vectors::norm( xWeights, xWeights, _xSize );
      Vectors::norm( zWeights, zWeights, _zSize );
   }

   // Update pointer to new weight vector for Z to monitor
   Vectors::copy( response, zWeights, _zSize );
}

void YArea::writeToDatabase( ofstream* database ) {
   database->write( (char*) &_numNeurons, sizeof(unsigned) );
   database->write( (char*) &_xSize, sizeof(unsigned) );
   for ( unsigned i = 0; i < _numNeurons; i++ ) {
      database->write( (char*) _xNeurons[i], sizeof(double) * _xSize );
   }

   database->write( (char*) &_zSize, sizeof(unsigned) );
   for ( unsigned i = 0; i < _numNeurons; i++ ) {
      database->write( (char*) _zNeurons[i], sizeof(double) * _zSize );
   }
}

YArea::YArea( ifstream* database, double* x, double* z ) {
   database->read( (char*) &_numNeurons, sizeof(unsigned) );

   _x = x;
   _z = z;

   database->read( (char*) &_xSize, sizeof(unsigned) );
   _xNeurons = allocNeuronBank( _xSize );
   for ( unsigned i = 0; i < _numNeurons; i++ ) {
      database->read( (char*) _xNeurons[i], sizeof(double) * _xSize );
   }

   database->read( (char*) &_zSize, sizeof(unsigned) );
   _zNeurons = allocNeuronBank( _zSize );
   for ( unsigned i = 0; i < _numNeurons; i++ ) {
      database->read( (char*) _zNeurons[i], sizeof(double) * _zSize );
   }

   _neuronalAges = new double[_numNeurons];
   _sampleX = new double[_xSize];
   _sampleZ = new double[_zSize];

   response = new double[_zSize];
}

/**
* Stores out a PGM file with the weights of a bank of neurons
* neuronType may be either 'X' or 'Z' referring to the X or Z neuron banks
**/
void YArea::saveNeuronBank( const char* fileName, char neuronType, unsigned stride ) {
   ofstream stem( fileName, ios::binary );

   unsigned size;
   double **neuronBank;

   switch ( neuronType ) {
   case 'X':
      size = _xSize;
      neuronBank = _xNeurons;
      break;
   case 'Z':
      size = _zSize;
      neuronBank = _zNeurons;
      break;
   default:
      fprintf( stderr, "Invalid neuron type '%c' selected", neuronType );
      return;
   }

   unsigned rowsCols = (unsigned) sqrt( _numNeurons );
   unsigned padding  = rowsCols - 1;
   unsigned totalStride = stride * rowsCols + padding;
   unsigned imageHeight = size / stride;
   unsigned totalHeight = imageHeight * rowsCols + padding;

   // Write out PGM header
   stem << "P5" << endl;
   stem << totalStride << endl;
   stem << totalHeight << endl;
   stem << WHITE << endl;

   char horizSpacer[totalStride];
   Vectors::fill( horizSpacer, (char)WHITE, totalStride );

   for ( unsigned row = 0; row < rowsCols; row++ ) {
      char rowImages[rowsCols][size];
      for ( unsigned col = 0; col < rowsCols; col++ ) {
         Vectors::scaleTo255( rowImages[col], neuronBank[row+col], size );
      }

      for ( unsigned line = 0; line < imageHeight; line++ ) {
         for ( unsigned image = 0; image < rowsCols - 1; image++ ) {
            stem.write( rowImages[image] + line, stride );
            stem.put( WHITE );
         }
         stem.write( rowImages[rowsCols - 1], stride );
      }
      
      if ( row != rowsCols - 1 ) { // Skip trailing spacer
         stem.write( horizSpacer, totalStride );
      }
   }

   stem.close();
}

void YArea::saveAges( const char* fileName ) {
   ofstream ages( fileName );
   
   unsigned rowsCols = (unsigned) sqrt( _numNeurons );

   for ( unsigned row = 0; row < rowsCols; row++ ) {
      for ( unsigned col = 0; col < rowsCols; col++ ) {
         ages << _neuronalAges[row+col] << '\t';
      }
      ages << endl;
   }

   ages.close();
}

