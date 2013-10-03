#ifndef SETTINGS_H
#define SETTINGS_H

typedef struct {
   char* listingFile;
   char* networkFile;
   char* outputFile;
   int   numEpochs;
} settings_t;

int areSettingsValid( settings_t* settings ) {
   return settings->listingFile != NULL &&
          settings->networkFile != NULL &&
          settings->outputFile  != NULL;
}

#endif
