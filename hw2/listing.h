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

   int load( Settings* settings );

   vector<char*>* getClass( int atPosition ) { return _listing->at( atPosition ); }
   int getNumClasses() { return _listing->size(); }

private:
   vector<vector<char*>*> *_listing;
};
