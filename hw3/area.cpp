#include "area.h"

Area::Area( unsigned neurons, Area *b, Area *t ) {
   _response = new double[neurons];
   _ages = new unsigned[neurons];

   _b = b;
   _synWeightB = new double[b->numNeurons()];

   _t = t;
   _synWeightT = new double[t->numNeurons()];

   _numNeurons = neurons;
}

Area::~Area() {
   delete [] _response;
   delete [] _ages;

   delete [] _synWeightB;
   delete [] _synWeightT;
}


