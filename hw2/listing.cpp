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

   for ( unsigned i = 0; i < _numClasses; i++ ) {
      delete _classes[i];
   }

   delete [] _listing;
   delete [] _classes;
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
      listing >> _numClasses;
      listing >> _size;

      _listing = new char*[_size];
      char** listingPointer = _listing;
      _classes = new imgClass_t*[_numClasses];
      for ( unsigned i = 0; i < _numClasses; i++ ) {
         _classes[i] = new imgClass_t;
         _classes[i]->ptr = listingPointer;
         listing >> _classes[i]->size;

         listingPointer += _classes[i]->size;
      }

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
