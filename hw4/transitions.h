/**
* Dennis Cornwell
* Class used in traning only to transition the Z area from state to state as a response to
* new X inputs. Holds a full mapping of the training state transition diagram.
**/

#ifndef TRANSITIONS_H
#define TRANSITIONS_H

#include "vocabulary.h"

#define FA_STATES 6

// State transitions for supervision
const unsigned TRANSITIONS[FA_STATES][VOCAB_SIZE] = { 
   { 0, 0, 5, 2, 0, 4, 0, 0, 0, 1 },
   { 2, 0, 5, 2, 0, 4, 0, 0, 1, 1 },
   { 0, 0, 5, 2, 3, 4, 3, 0, 2, 1 },
   { 0, 0, 5, 2, 0, 4, 0, 0, 3, 1 },
   { 0, 0, 5, 2, 0, 4, 0, 5, 4, 1 },
   { 0, 0, 5, 2, 0, 4, 0, 0, 5, 1 }
};

class Transitions {
public:

   static int getNextState( double* currentState, unsigned vocabId ) {
      unsigned lastState = 0;
      for ( unsigned i = 0; i < FA_STATES; i++ ) {
         if ( currentState[i] > 0 ) {
            lastState = i;
            break;
         }
      }

      return TRANSITIONS[lastState][vocabId-1];
   }

};

#endif
