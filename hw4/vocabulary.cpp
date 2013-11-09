#include "vocabulary.h"
#include <fstream>
#include <string>

using namespace std;

const char* Vocabulary::VOCABULARY[VOCAB_SIZE] = {
   "cat",    "full",
   "hungry", "kitten",
   "looks",  "meal",
   "stares", "time",
   "well",   "young"
};

Vocabulary::Vocabulary( const char * fileName ) : _file( fileName ) {}

Vocabulary::~Vocabulary() {
   _file.close();
}

void indexToBinaryPattern( unsigned match, double* neurons, unsigned size ) {
   for ( unsigned i = 0; i < size; i++ ) {
      neurons[i] = ( match >> ( size - i - 1 ) ) & 1;
   }
}

// Get the 1 based index of the next word
bool Vocabulary::encodeNextWord( double* neurons, unsigned size ) {
   string word;
   _file >> word;

   printf( "Testing word %s\n", word.c_str() );

   unsigned result = false;

   for ( unsigned i = 0; i < VOCAB_SIZE; i++ ) {
      if ( !strcmp( word.c_str(), VOCABULARY[i] ) ) {
         indexToBinaryPattern( i+1, neurons, size );

         result = true;
      }
   }

   return result;
}
