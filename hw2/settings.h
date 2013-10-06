#ifndef SETTINGS_H
#define SETTINGS_H

#include <cstddef>

class Settings {
public:
   char* listingFile;
   char* networkFile;
   char* outputFile;
   int   numEpochs;

   Settings() {
      listingFile = NULL;
      networkFile = NULL;
      outputFile  = NULL;
      numEpochs   = 1;
   }

   bool areValid() {
      return listingFile != NULL &&
             networkFile != NULL &&
             outputFile  != NULL;
   }
};

#endif
