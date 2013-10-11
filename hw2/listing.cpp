#include "listing.h"
#include <iostream>
#include <fstream>
#include <string>
#include <stdio.h>

Listing::Listing() {
   _listing = new vector<vector<char*>*>();
}

Listing::~Listing() {
   for ( unsigned i = 0; i < _listing->size(); i++ ) {
      for ( unsigned j = 0; j < _listing->at( i )->size(); j++ ) {
         delete [] _listing->at( i )->at( j );
      }
      delete _listing->at( i );
   }

   delete _listing;
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
      unsigned numClasses = 0;
      unsigned numImages = 0;
      listing >> numClasses;
      listing >> numImages;

      unsigned classSizes[numClasses];
      for ( unsigned i = 0; i < numClasses; i++ ) {
         listing >> classSizes[i];
      }

      for ( unsigned c = 0; c < numClasses; c++ ) {
         vector<char*>* clazz = new vector<char*>();
         
         for ( unsigned i = 0; i < classSizes[c]; i++ ) {
            char* filePath = new char[FILE_NAME_MAX];
            char fileName[FILE_NAME_MAX];
            
            listing >> fileName;
            // Concat file name with directory path
            sprintf( filePath, "%s/%s", imagesDir->c_str(), fileName );
            
            clazz->push_back( filePath );
         }

         _listing->push_back( clazz );
      }

   } else {
      error = LOAD_OPEN_FILE_FAIL;
   }

   delete imagesDir;

   return error;
}
