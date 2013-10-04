#include "listing.h"
#include <iostream>
#include <fstream>

Listing::Listing() {
   _listing = new vector<vector<string*>*>();
}

Listing::~Listing() {
   for ( unsigned i = 0; i < _listing->size(); i++ ) {
      for ( unsigned j = 0; j < _listing->at( i )->size(); j++ ) {
         delete _listing->at( i )->at( j );
      }
      delete _listing->at( i );
   }

   delete _listing;
}

int Listing::load( Settings* settings ) {
   ifstream listing( settings->listingFile );

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
         vector<string*>* clazz = new vector<string*>();
         
         for ( unsigned i = 0; i < classSizes[c]; i++ ) {
            string* fileName = new string();
            listing >> *fileName;
            clazz->push_back( fileName );
         }

         _listing->push_back( clazz );
      }

      return LOAD_OK;
   } else {
      return LOAD_OPEN_FILE_FAIL;
   }
}
