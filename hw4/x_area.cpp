#include "x_area.h"

XArea::XArea( unsigned numNeurons ) {
   response = new double[numNeurons];
}

XArea::~XArea() {
   delete [] response;
}

