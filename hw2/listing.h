#ifndef LISTING_H
#define LISTING_H

#include "settings.h"
#include <vector>

#define FILE_NAME_MAX 256

#define LOAD_OK 0
#define LOAD_OPEN_FILE_FAIL 1

using namespace std;

struct imgClass_t {
   char** ptr;
   size_t size;
};

class Listing {
public:
   Listing();
   ~Listing();

   int         load( char* file );
   unsigned    size() { return _size; } 
   unsigned    numClasses() { return _numClasses; }
   char*&      operator[](unsigned idx) { return _listing[idx]; }
   imgClass_t* classAt( unsigned i ) { return _classes[i]; }

private:
   unsigned     _size;
   unsigned     _numClasses;
   imgClass_t** _classes;
   char**       _listing;
};

#endif
