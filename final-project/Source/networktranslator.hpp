/**
 * @file    networktranslator.hpp
 * @author  Austin Gregory
 * @created 2013-11-13
 */

#ifndef NETWORKTRANSLATOR_HPP
#define NETWORKTRANSLATOR_HPP

#include "vector.hpp"

using namespace std;

class NetworkTranslator
{
public:
	NetworkTranslator();
	~NetworkTranslator();
	
	unsigned getNumberOfInputNeurons() const;
	unsigned getNumberOfOutputNeurons() const;
	
	Vector translateInput(string input);
	string translateInput(Vector* input);
	Vector translateOutput(string output);
	string translateOutput(Vector* output);
	Vector findNextOutput(Vector* currentOutput, Vector* input); // For training only.
	Vector findNearestActualInput(Vector* input);
	Vector findNearestActualOutput(Vector* output);

private:
	NetworkTranslator(const NetworkTranslator& networkTranslator);
	
	NetworkTranslator& operator=(const NetworkTranslator& networkTranslator);
	
	unsigned findInputIndex(string input);
	unsigned findOutputIndex(string output);
	
	// Both neuron counts must be greater than or equal to 1!
	static const unsigned _NUM_IN_NEURONS = 4;
	static const unsigned _NUM_OUT_NEURONS = 6;
	
	static const unsigned _NUM_IN_POSSIBLE = 11;
	static const unsigned _NUM_OUT_POSSIBLE = 6;
	
	static const string _INPUTS[_NUM_IN_POSSIBLE];
	
	static const string _OUTPUTS[_NUM_OUT_POSSIBLE];
	
	// Entry [output index][input index - 1] is the output index for the next state.
	static const unsigned _IN_OUT_MAP[_NUM_OUT_POSSIBLE][_NUM_IN_POSSIBLE - 1];
};

#endif
