#include "settings.h"
#include <string>
#include <vector>

#define LOAD_OK 0
#define LOAD_OPEN_FILE_FAIL 1

using namespace std;

class Listing {
public:
   Listing();
   ~Listing();

   int load( Settings* settings );

   vector<string*>* getClass( int atPosition ) { return _listing->at( atPosition ); }
   int getNumClasses() { return _listing->size(); }

private:
   vector<vector<string*>*> *_listing;
};
