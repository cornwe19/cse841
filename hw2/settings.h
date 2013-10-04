#ifndef SETTINGS_H
#define SETTINGS_H

class Settings {
   public:
      char* listingFile;
      char* networkFile;
      char* outputFile;
      int   numEpochs;

      bool areValid() {
         return listingFile != NULL &&
                networkFile != NULL &&
                outputFile  != NULL;
      }
};

#endif
