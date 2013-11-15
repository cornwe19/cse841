#include "vocabulary.h"
#include <fstream>
#include <string>

using namespace std;

const char* Vocabulary::VOCABULARY[VOCAB_SIZE] = {
   "_",
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

unsigned Vocabulary::nextWordId() {
   string word;
   _file >> word;


   unsigned result = VOCAB_SIZE;

   if ( word.size() > 0  ) {
      printf( "Read word: '%s'\n", word.c_str() );
      for ( unsigned i = 0; i < VOCAB_SIZE; i++ ) {
         if ( !strcmp( word.c_str(), VOCABULARY[i] ) ) {
            result = i;
            break;
         }
      }
   }

   return result;
}
