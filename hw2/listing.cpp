#include "listing.h"
#include <iostream>
#include <fstream>
#include <string>
#include <stdio.h>

Listing::Listing() {
   _listing = NULL;
   _size    = 0;
}

Listing::~Listing() {
   for ( unsigned i = 0; i < _size; i++ ) {
      delete [] _listing[i];
   }

   delete [] _listing;
}

string* getImagesDir( char* file ) {
   string fullPath( file );
   return new string( fullPath.substr( 0, fullPath.find_last_of( "/" ) ) );
}

int Listing::load( char* file ) {
   ifstream listing( file );
   string* imagesDir = getImagesDir( file );
   int error = LOAD_OK;

   if ( listing.is_open() ) {
      unsigned numClasses = 0, scratch;
      listing >> numClasses;
      listing >> _size;

      for ( unsigned i = 0; i < numClasses; i++ ) {
         listing >> scratch;
      }

      _listing = new char*[_size];
      for ( unsigned i = 0; i < _size; i++ ) {
         _listing[i] = new char[FILE_NAME_MAX];
         char fileName[FILE_NAME_MAX];
         listing >> fileName;

         sprintf( _listing[i], "%s/%s", imagesDir->c_str(), fileName );
      }
   } else {
      error = LOAD_OPEN_FILE_FAIL;
   }

   delete imagesDir;

   return error;
}
