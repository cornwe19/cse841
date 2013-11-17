#ifndef PGM_H
#define PGM_H

#define IMG_BLACK 0
#define IMG_WHITE 255

class PGM {
public:
   static void saveAggregateImage( const char* fileName, double** data, unsigned singleImageSize, unsigned rows, unsigned cols, unsigned stride );
};

#endif
