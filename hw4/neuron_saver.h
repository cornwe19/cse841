#ifndef NEURON_SAVER_H
#define NEURON_SAVER_H

#define IMG_BLACK 0
#define IMG_WHITE 255

class NeuronSaver {
public:
   static void saveAggregateImage( const char* fileName, double** data, unsigned singleImageSize, unsigned rows, unsigned cols, unsigned stride );
   static void saveAges( const char* fileName, double* data, unsigned rows, unsigned cols );
};

#endif
