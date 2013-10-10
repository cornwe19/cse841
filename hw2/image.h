#ifndef IMAGE_H
#define IMAGE_H

#define IMAGE_SIZE 64*88

class Image {
public:
   Image( char* fileName );
   ~Image();

   char* getClassName() { return _className; };
   char* getFileName() { return _fileName; };

   double euclideanDistanceTo( Image* image );
   double* getData() { return _data; };

   Image *bestMatch;
   
private:
   char*   _fileName;
   char*   _className;
   double* _data;
};

#endif
