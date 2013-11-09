#ifndef VOCABULARY_H
#define VOCABULARY_H

#include <fstream>

#define VOCAB_SIZE 10
#define WORDSIZE_MAX 16

class Vocabulary {
public:
   Vocabulary( const char* fileName );
   ~Vocabulary();

   static const char* VOCABULARY[VOCAB_SIZE];

   bool encodeNextWord( double* neurons, unsigned size );

private:
   std::ifstream _file;
   
};

#endif
