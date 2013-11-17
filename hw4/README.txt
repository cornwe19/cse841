CSE 841 Homework 4
Dennis Cornwell

Source files:
 - makefile
 - dntext.cpp
 - settings.h
 - x_area.h
 - x_area.cpp
 - y_area.h
 - y_area.cpp
 - z_area.h
 - z_area.cpp
 - vectors.h
 - vectors.cpp
 - neuron_saver.h
 - neuron_saver.cpp
 - vocabulary.h
 - vocabulary.cpp
 - transitions.h

Compilation and running:
 - run 'make' from the project root to make the project
    - note that a 'bin' directory will be created in the process of making
 - run ./dntext -f TRAINING_FILE -d DATABASE_FILE -o OUTPUT_STEM -l -Y NUM_NEURONS for training
     - NOTE: NUM_NEURONS corresponds to the dimensionality of the square matrix of neurons in the Y area (n = NUM_NEURONS^2). For this project a minimum of 8 is required for Y to learn perfectly
     - Running dntext in training mode results in $STEM-xy.pgm, $STEM-zy.pgm, $STEM-yx.pgm, $STEM-yz.pgm, $STEM-ax.txt, $STEM-ay.txt and $STEM-az.txt being generated in the project root directory
 - run ./dntext -f TESTING_FILE -d DATABASE_FILE -o OUTPUT_FILE for testing
     - Running dn in testing mode results in OUTPUT_FILE being created with the results of the test run

Input File formats:
 - Training and testing
    - Files are read from the vocabulary of 10 words. It is expected that each word is space delimited and no other punctuation, capitalization, line breaks, or whitespace exists in the file
    - Example: "cat hungry hungry full"
 - Thinking
    - File format from training and testing applies. Thinking is denoted with an underscore before the expected word. Words beginning with an underscore are not provided to X allowing it to think independently. The expected word is tested against X's prediction for correctness
    - Example: "cat _hungry hungry _full" where the first 'hungry' and 'full' are expected, but not provided to X

