/**
* Dennis Cornwell
* Defines a helper class for reading text files and mapping their input to a integer
* index based vocabulary. Note that 0 means a blank was read and that the DN should 'think'
* for itself.
**/

#ifndef VOCABULARY_H
#define VOCABULARY_H

#include <fstream>
#include <cstdlib>

#define VOCAB_SIZE 11
#define WORDSIZE_MAX 16

class Vocabulary {
public:
   Vocabulary( const char* fileName );
   ~Vocabulary();

   static const char* VOCABULARY[VOCAB_SIZE];
   unsigned unused;
   /** 
   * ID of next vocab word in file
   * Note: a result of VOCAB_SIZE means there is no more input
   **/
   unsigned nextWordId( unsigned* expected = NULL );

private:
   std::ifstream   _file;
};

#endif
