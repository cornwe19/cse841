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

// Returns a result which may be 0 and an expected value which is non-zero
//  - In the case of thinking, expected is the expected word and result is 0.
unsigned Vocabulary::nextWordId( unsigned* expected ) {
   string word;
   _file >> word;

   // Use a dummy variable for expectation if it wasn't set
   unsigned dontCareExpectation;
   if ( expected == NULL ) {
      expected = &dontCareExpectation;
   }

   *expected = VOCAB_SIZE;

   const char* test = word.c_str();
   bool isThinking = word.c_str()[0] == '_';
   if ( isThinking ) {
      test++;
   }

   if ( word.size() > 0  ) {
      for ( unsigned i = 0; i < VOCAB_SIZE; i++ ) {
         if ( !strcmp( test, VOCABULARY[i] ) ) {
            *expected = i;
            break;
         }
      }
   }
   
   if ( isThinking ) {
      return 0;
   } else {
      return *expected;
   }
}
