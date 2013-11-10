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

   static void updateState( double* Z, double* X, unsigned xSize ) {
      // Reconstruct vocabulary ID
      unsigned vocabId = 0;
      for ( unsigned i = 0; i < xSize; i++ ) {
         vocabId = vocabId << 1;
         vocabId += X[i];
      }

      // Read current state
      unsigned lastState = 0;
      for ( unsigned i = 0; i < FA_STATES; i++ ) {
         if ( Z[i] > 0 ) {
            lastState = i;
            break;
         }
      }

      // 0 out last state, follow transition, turn on new state
      Z[lastState] = 0;
      unsigned nextState = TRANSITIONS[lastState][vocabId-1];
      Z[nextState] = 1;
   }

};

#endif
