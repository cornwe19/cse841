/**
 * @file    networktranslator.hpp
 * @author  Austin Gregory
 * @created 2013-11-13
 */

#ifndef NETWORKTRANSLATOR_HPP
#define NETWORKTRANSLATOR_HPP

#include <vector>

#include "vector.hpp"

using namespace std;

class NetworkTranslator
{
public:
	NetworkTranslator();
	~NetworkTranslator();
	
	unsigned getNumberOfInputNeurons() const;
	unsigned getNumberOfOutputNeurons() const;
	void printInputs() const;
	void printOutputs() const;
	
	void addInputIfNotFound(string input);
	void addOutputIfNotFound(string output);
	
	Vector translateInput(string input);
	string translateInput(Vector input);
	string translateInput(Vector* input);
	
	Vector translateOutput(string output);
	string translateOutput(Vector output);
	string translateOutput(Vector* output);
	
	Vector findNearestActualInput(Vector input);
	Vector findNearestActualInput(Vector* input);
	
	Vector findNearestActualOutput(Vector output);
	Vector findNearestActualOutput(Vector* output);

private:
	NetworkTranslator(const NetworkTranslator& networkTranslator);
	
	NetworkTranslator& operator=(const NetworkTranslator& networkTranslator);
	
	unsigned findInputIndex(string input);
	unsigned findOutputIndex(string output);
	
	// Both neuron counts must be greater than or equal to 1!
	static const unsigned _NUM_IN_NEURONS = 10;
	static const unsigned _NUM_OUT_NEURONS = 25;
	
	static const unsigned _NUM_IN_POSSIBLE = 1000;
	static const unsigned _NUM_OUT_POSSIBLE = 25;
	
	vector<string> _inputs;
	vector<string> _outputs;
};

#endif
