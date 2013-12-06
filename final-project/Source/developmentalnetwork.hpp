/**
 * @file    developmentalnetwork.hpp
 * @author  Austin Gregory
 * @created 2013-10-30
 */

#ifndef DEVELOPMENTALNETWORK_HPP
#define DEVELOPMENTALNETWORK_HPP

#include <fstream>
#include <vector>

#include "networktranslator.hpp"

using namespace std;

class DevelopmentalNetwork
{
public:
	DevelopmentalNetwork(unsigned hiddenNeuronCap);
	~DevelopmentalNetwork();
	
	void save(string destinationFileName);
	void load(string sourceFileName);
	
	void process();
	
	void printHiddenLayer() const;

private:
	struct OutputLayer
	{
		Vector* currentOutput;
		Vector* nextOutput;
	};
	
	struct HiddenNeuron
	{
		Vector* currentOutputPart;
		Vector* nextOutputPart;
		Vector* inputPart;
		
		unsigned age;
	};
	
	DevelopmentalNetwork();
	DevelopmentalNetwork(const DevelopmentalNetwork& developmentalNetwork);
	
	DevelopmentalNetwork& operator=(const DevelopmentalNetwork& developmentalNetwork);
	
	void processInput(string word, bool isTraining);
	unsigned selectHiddenNeuron(bool isTraining);
	
	OutputLayer _outputLayer;
	vector<HiddenNeuron*> _hiddenLayer;
	Vector* _inputLayer;
	
	NetworkTranslator _translator;
	unsigned _hiddenNeuronCap;
};

#endif
