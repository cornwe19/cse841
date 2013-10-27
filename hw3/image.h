/**
* Dennis Cornwell
* Image class used as a helper to wrap loading raw pgm files
*  - Supplies class and file names as well as file data
**/

#ifndef IMAGE_H
#define IMAGE_H

#define IMAGE_SIZE 64*88
#define CLASS_MAX 128

class Image {
public:
   Image( char* fileName );
   ~Image();

   char* getClassName() { return _className; };
   char* getFileName() { return _fileName; };

   double* getData() { return _data; };

   double& operator[]( unsigned i ) { return _data[i]; }

   static char* parseClassName( char *fileName );

private:
   char*   _fileName;
   char*   _className;
   double* _data;
};

#endif
