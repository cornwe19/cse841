#ifndef VOCABULARY_H
#define VOCABULARY_H

#include <fstream>

#define VOCAB_SIZE 11
#define WORDSIZE_MAX 16

class Vocabulary {
public:
   Vocabulary( const char* fileName );
   ~Vocabulary();

   static const char* VOCABULARY[VOCAB_SIZE];

   /** 
   * ID of next vocab word in file
   * Note: a result of VOCAB_SIZE means there is no more input
   **/
   unsigned nextWordId();

private:
   std::ifstream _file;
   
};

#endif
