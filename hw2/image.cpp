#include "image.h"
#include <fstream>
#include <cctype>
#include <cstring>
#include <cstdlib>
#include <cmath>

#include <stdio.h>

using namespace std;

char* Image::parseClassName( char* fileName ) {
   char* startPos = strrchr( fileName, '/' );

   if ( startPos == NULL ) {
      startPos = fileName;
   } else  {
      startPos++; // Skip the '/'
   }

   char* className = new char[CLASS_MAX];
   char* classPtr = className;
   for ( char* i = startPos; i != NULL ; i++ ) {
      if ( toupper( *i ) != *i ) {
         *classPtr = *i;
         classPtr++;
      } else {
         *classPtr = NULL;
         break;
      }
   }

   return className;
}

Image::Image( char* fileName ) {
   _fileName = fileName;
   _data = new double[IMAGE_SIZE];

   _className = parseClassName( fileName );

   // Load in char data as a vector of doubles
   char scratch[IMAGE_SIZE];
   ifstream image( fileName, ios::binary );
   image.read( scratch, IMAGE_SIZE );
   image.close();

   for ( int i = 0; i < IMAGE_SIZE; i++ ) {
      _data[i] = (double)scratch[i];
   }
}

Image::~Image() {
   delete [] _data;
   delete [] _className;
}

