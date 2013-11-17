CSE 841 Homework 4
Dennis Cornwell

Source files:
 - makefile
 - listing.h
 - listing.cpp
 - image.h
 - image.cpp
 - dntext.cpp
 - settings.h
 - y_area.h
 - y_area.cpp
 - z_area.h
 - z_area.cpp
 - vectors.h
 - vectors.cpp

Compilation and running:
 - run 'make' from the project root to make the project
    - note that a 'bin' directory will be created in the process of making
 - run ./dntext -f TRAINING_LIST -d DATABASE_FILE -o OUTPUT_FILE -l NUM_EPOCHS -Y NUM_NEURONS for training
     - NOTE: NUM_NEURONS corresponds to the dimensionality of the square matrix of neurons in the Y area (n = NUM_NEURONS^2)
     - Running dntext in training mode results in $STEM-xy.pgm, $STEM-zy.pgm, $STEM-yx.pgm, $STEM-yz.pgm, $STEM-ax.txt, $STEM-ay.txt and $STEM-az.txt being generated in the project root directory
 - run ./dntext -f TESTING_LIST -d DATABASE_FILE -o OUTPUT_FILE for testing
     - Running dn in testing mode results in OUTPUT_FILE being created with the results of the test run

