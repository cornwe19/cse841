#ifndef X_AREA_H
#define X_AREA_H

class XArea {
public:
   XArea( unsigned numNeurons );
   ~XArea();

   double* response;
};

#endif
