#include "y_area.h"
#include "vectors.h"
#include "pgm.h"
#include <stdlib.h>
#include <stdio.h>
#include <fstream>
#include <cmath>

using namespace std;

YArea::YArea( unsigned numNeurons, double *x, const unsigned xSize, double *z, const unsigned zSize ) {
   _numNeurons = numNeurons;
   _xSize = xSize;
   _zSize = zSize;

   _x = x;
   _z = z;

   _xNeurons = allocNeuronBank( xSize );
   _zNeurons = allocNeuronBank( zSize );
   _numInitialized = 0;

   response = new double[_xSize + _zSize];

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

void YArea::computePreresponse( bool isFrozen ) {
   // Find matching neuron
   Vectors::copy( _sampleX, _x, _xSize );
   Vectors::copy( _sampleZ, _z, _zSize );
   
   if ( isFrozen || _numInitialized == _numNeurons ) {
      double bestFit = 0;
      for ( unsigned i = 0; i < _numInitialized; i++ ) {
         double xFit = Vectors::dot( _xNeurons[i], _sampleX, _xSize );
         double zFit = Vectors::dot( _zNeurons[i], _sampleZ, _zSize );
         
         if ( xFit + zFit > bestFit ) {
            bestFit = xFit + zFit;
            _neuronalMatch = i;
         }
      }
   } else {
      bool foundMatch = false;
      for ( unsigned i = 0; i < _numInitialized; i++ ) {
         double xFit = Vectors::dot( _xNeurons[i], _sampleX, _xSize );
         double zFit = Vectors::dot( _zNeurons[i], _sampleZ, _zSize );

         // Smaller than epsilon
         if ( 2.0 - ( xFit + zFit ) < 0.0000001 ) {
            _neuronalMatch = i;
            foundMatch = true;
         }
      }
      
      if ( !foundMatch ) {
         _neuronalMatch = _numInitialized;
         Vectors::copy( _xNeurons[_numInitialized], _sampleX, _xSize );
         Vectors::copy( _zNeurons[_numInitialized], _sampleZ, _zSize );
         _numInitialized++;
      }
   }
}

void YArea::update( bool frozen ) {
   double* zWeights = _zNeurons[_neuronalMatch];
   double* xWeights = _xNeurons[_neuronalMatch];

   // Pretty good chance we won't encounter a situation
   // in this homework where we hit this code.
   if ( !frozen && _numInitialized == _numNeurons ) {
      // Update weight vectors for matched neuron
      double age = ++_neuronalAges[_neuronalMatch];
      double w1 = ( age - 1.0 ) / 1.0;
      double w2 = 1.0 / age;

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

   // Copy weights into response vectors
   Vectors::copy( response, xWeights, _xSize );
   Vectors::copy( response + _xSize, zWeights, _zSize );

   // Renormalize response from Y
   Vectors::norm( response, response, _xSize + _zSize );
}

void YArea::writeToDatabase( ofstream &database ) {
   database.write( (char*) &_numNeurons, sizeof(unsigned) );
   database.write( (char*) &_numInitialized, sizeof(unsigned) );
   database.write( (char*) &_xSize, sizeof(unsigned) );
   for ( unsigned i = 0; i < _numNeurons; i++ ) {
      database.write( (char*) _xNeurons[i], sizeof(double) * _xSize );
   }

   database.write( (char*) &_zSize, sizeof(unsigned) );
   for ( unsigned i = 0; i < _numNeurons; i++ ) {
      database.write( (char*) _zNeurons[i], sizeof(double) * _zSize );
   }
}

YArea::YArea( ifstream &database, double* x, double* z ) {
   database.read( (char*) &_numNeurons, sizeof(unsigned) );
   database.read( (char*) &_numInitialized, sizeof(unsigned) );

   _x = x;
   _z = z;

   database.read( (char*) &_xSize, sizeof(unsigned) );
   _xNeurons = allocNeuronBank( _xSize );
   for ( unsigned i = 0; i < _numNeurons; i++ ) {
      database.read( (char*) _xNeurons[i], sizeof(double) * _xSize );
   }

   database.read( (char*) &_zSize, sizeof(unsigned) );
   _zNeurons = allocNeuronBank( _zSize );
   for ( unsigned i = 0; i < _numNeurons; i++ ) {
      database.read( (char*) _zNeurons[i], sizeof(double) * _zSize );
   }

   _neuronalAges = new double[_numNeurons];
   _sampleX = new double[_xSize];
   _sampleZ = new double[_zSize];

   response = new double[_xSize + _zSize];
}

/**
* Stores out a PGM file with the weights of a bank of neurons
* neuronType may be either 'X' or 'Z' referring to the X or Z neuron banks
**/
void YArea::saveNeuronBank( const char* fileName, char neuronType, unsigned stride ) {
   ofstream out( fileName, ios::binary );

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
  
   PGM::saveAggregateImage( fileName, neuronBank, size, rowsCols, rowsCols, stride );
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

