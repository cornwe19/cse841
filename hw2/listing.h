#include "settings.h"
#include <vector>

#define FILE_NAME_MAX 256

#define LOAD_OK 0
#define LOAD_OPEN_FILE_FAIL 1

using namespace std;

class Listing {
public:
   Listing();
   ~Listing();

   int      load( char* file );
   unsigned size() { return _size; } 
   char*&   operator[](unsigned idx) { return _listing[idx]; }

private:
   unsigned _size;
   char**   _listing;
};
