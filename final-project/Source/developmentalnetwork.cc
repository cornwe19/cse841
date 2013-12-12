/**
 * @file    developmentalnetwork.cc
 * @author  Austin Gregory
 * @created 2013-10-30
 */

#include "developmentalnetwork.hpp"
#include <sys/select.h>
#include <sys/types.h>

DevelopmentalNetwork::DevelopmentalNetwork( string startingOutput, unsigned hiddenNeuronCap )
{
	_translator.addOutputIfNotFound(startingOutput);
	
	Vector normedStartingOutput(_translator.translateOutput(startingOutput));
	normedStartingOutput /= normedStartingOutput.calculateNorm();
	
	_outputLayer.currentOutput = new Vector(normedStartingOutput);
	_outputLayer.nextOutput = new Vector(_translator.getNumberOfOutputNeurons());
	
	_inputLayer = new Vector(_translator.getNumberOfInputNeurons());
	
	_hiddenNeuronCap = hiddenNeuronCap;
}

DevelopmentalNetwork::~DevelopmentalNetwork()
{
	delete _outputLayer.currentOutput;
	delete _outputLayer.nextOutput;
	
	for (unsigned i = 0; i < _hiddenLayer.size(); ++i)
	{
		delete _hiddenLayer[i]->currentOutputPart;
		delete _hiddenLayer[i]->nextOutputPart;
		delete _hiddenLayer[i]->inputPart;
		
		delete _hiddenLayer[i];
	}
	
	delete _inputLayer;
}





void DevelopmentalNetwork::save(string destinationFileName)
{
	unsigned numberOfNeurons = _hiddenLayer.size();
	
	ofstream destinationFile(destinationFileName.c_str(), ios::out | ios::binary);
	
	destinationFile.write(reinterpret_cast<char*>(&numberOfNeurons), sizeof(unsigned));
	
	for (unsigned i = 0; i < _hiddenLayer.size(); ++i)
	{
		for (unsigned j = 0; j < _translator.getNumberOfOutputNeurons(); ++j)
		{
			destinationFile.write(reinterpret_cast<char*>(&_hiddenLayer[i]->
				currentOutputPart->at(j)), sizeof(double));
		}
		
		for (unsigned j = 0; j < _translator.getNumberOfOutputNeurons(); ++j)
		{
			destinationFile.write(reinterpret_cast<char*>(&_hiddenLayer[i]->
				nextOutputPart->at(j)), sizeof(double));
		}
		
		for (unsigned j = 0; j < _translator.getNumberOfInputNeurons(); ++j)
		{
			destinationFile.write(reinterpret_cast<char*>(&_hiddenLayer[i]->inputPart->at(j)), 
				sizeof(double));
		}
		
		destinationFile.write(reinterpret_cast<char*>(&_hiddenLayer[i]->age), sizeof(unsigned));
	}
	
	destinationFile.close();
}

void DevelopmentalNetwork::load(string sourceFileName)
{
	unsigned numberOfNeurons;
	
	ifstream sourceFile(sourceFileName.c_str(), ios::in | ios::binary);

	sourceFile.read(reinterpret_cast<char*>(&numberOfNeurons), sizeof(unsigned));
	
	for (unsigned i = 0; i < numberOfNeurons; ++i)
	{
		HiddenNeuron* newNeuron = new HiddenNeuron();
		newNeuron->currentOutputPart = new Vector(_translator.getNumberOfOutputNeurons());
		newNeuron->nextOutputPart = new Vector(_translator.getNumberOfOutputNeurons());
		newNeuron->inputPart = new Vector(_translator.getNumberOfInputNeurons());
		
		for (unsigned j = 0; j < _translator.getNumberOfOutputNeurons(); ++j)
		{
			sourceFile.read(reinterpret_cast<char*>(&newNeuron->currentOutputPart->at(j)), 
				sizeof(double));
		}
		
		for (unsigned j = 0; j < _translator.getNumberOfOutputNeurons(); ++j)
		{
			sourceFile.read(reinterpret_cast<char*>(&newNeuron->nextOutputPart->at(j)), 
				sizeof(double));
		}
		
		for (unsigned j = 0; j < _translator.getNumberOfInputNeurons(); ++j)
		{
			sourceFile.read(reinterpret_cast<char*>(&newNeuron->inputPart->at(j)), 
				sizeof(double));
		}
		
		sourceFile.read(reinterpret_cast<char*>(&newNeuron->age), sizeof(unsigned));
		
		_hiddenLayer.push_back(newNeuron);
	}
	
	sourceFile.close();
}





void DevelopmentalNetwork::process()
{
	fd_set readset;
	FD_ZERO( &readset );

	timeval  userTimeout;
	userTimeout.tv_sec = 5; // Give the user 5 seconds to enter some input

	timeval* timeout = NULL;
	
	while ( true )
	{
		FD_SET( 0, &readset ); // Wait on stdin
		unsigned fd_max = 1; // 1 + largest fd to wait on

		string input, output;
		
		if ( select( fd_max, &readset, NULL, NULL, timeout ) != 0 )
		{
			// User entered input, read it and run it through the DN
			string line;
			getline( cin, line );

			size_t split = line.find( ' ' );
			input = line.substr( 0, split );

			if ( split < line.size() ) 
			{
			   output = line.substr( split, line.size() );
			}
			else
			{
			   output = "_";
			}

			if ( input.compare( "q" ) == 0 )
			{
			   cout << "Exiting..." << endl;
			   break;
			}
		}
		else
		{
			 // We timed out, let the machine think freely
			 input = "_";
			 output = "_";
		}

		timeout = &userTimeout; 
		
		processUserInput(input, output);
	}
}





void DevelopmentalNetwork::printHiddenLayer() const
{
	cout << "HIDDEN LAYER: " << _hiddenLayer.size() << " NEURONS" << endl << endl;
	
	for (unsigned i = 0; i < _hiddenLayer.size(); ++i)
	{
		cout << "NEURON " << i << ":" << endl;
		cout << "Previous Output Part: " << *_hiddenLayer[i]->currentOutputPart << endl;
		cout << "Next Output Part: " << *_hiddenLayer[i]->nextOutputPart << endl;
		cout << "Input Part: " << *_hiddenLayer[i]->inputPart << endl;
		cout << endl;
	}
}





DevelopmentalNetwork::DevelopmentalNetwork()
{
}

DevelopmentalNetwork::DevelopmentalNetwork(const DevelopmentalNetwork& developmentalNetwork)
{
}





DevelopmentalNetwork& DevelopmentalNetwork::operator=(const DevelopmentalNetwork& 
	developementalNetwork)
{
	return *this;
}





void DevelopmentalNetwork::processUserInput(string input, string output)
{
	bool isTraining = (input != "_" && output != "_");
	bool isTesting = (input != "_" && output == "_");
	bool isThinking = (input == "_" && output == "_");
	bool isGoto = (!isTraining && !isTesting && !isThinking);
	
	_translator.addInputIfNotFound(input);
	_translator.addOutputIfNotFound(output);
	
	Vector normedInput(_translator.translateInput(input));
	if (normedInput.calculateNorm() > 0)
	{
		normedInput /= normedInput.calculateNorm();
	}
	
	Vector normedOutput(_translator.translateOutput(output));
	normedOutput /= normedOutput.calculateNorm();
	
	if ( isTraining )
	{
		cout << "Training..." << endl;
		cout << "Processing input: " << input << endl;
		cout << "Processing output: " << output << endl;
	}
	else if ( isTesting )
	{
		cout << "Testing..." << endl;
		cout << "Processing input: " << input << endl;
	}
	else if ( isThinking )
	{
		cout << "Thinking..." << endl;
	}
	else
	{
		cout << "Setting current state..." << endl;
		
		*_outputLayer.currentOutput = normedOutput;
		
		Vector nearestCurrentOutput(_translator.findNearestActualOutput(
			_outputLayer.currentOutput));
		
		cout << "Current state: " << _translator.translateOutput(nearestCurrentOutput);
		cout << endl << endl;
	}
	
	if (!isGoto)
	{
		processNetworkInput( normedInput, normedOutput, isTraining, isTesting, isThinking );

		Vector nearestCurrentOutput(_translator.findNearestActualOutput(
			_outputLayer.currentOutput));
			
		Vector nearestNextOutput(_translator.findNearestActualOutput(
			_outputLayer.nextOutput));
	
		cout << "Transition: " << _translator.translateOutput(nearestCurrentOutput);
		cout << " -> " << _translator.translateOutput(nearestNextOutput);
		cout << endl << endl;

		*_outputLayer.currentOutput = *_outputLayer.nextOutput;
	}
}

void DevelopmentalNetwork::processNetworkInput(Vector input, Vector output, bool isTraining, 
	bool isTesting, bool isThinking)
{
	*_inputLayer = input;
	
	if (isTraining || isTesting)
	{
		unsigned selectedHiddenNeuronIndex = selectHiddenNeuron(isTraining);
		bool createdNewNeuron = false;
		
		if (selectedHiddenNeuronIndex == _hiddenNeuronCap)
		{
			HiddenNeuron* newNeuron = new HiddenNeuron();
			newNeuron->currentOutputPart = new Vector(*_outputLayer.currentOutput);
			newNeuron->inputPart = new Vector(*_inputLayer);
			newNeuron->nextOutputPart = new Vector(output);
			newNeuron->age = 1;
			newNeuron->frequency = 1;
		
			_hiddenLayer.push_back(newNeuron);
		
			selectedHiddenNeuronIndex = _hiddenLayer.size() - 1;
			createdNewNeuron = true;
		}
		
		if (!createdNewNeuron && isTraining)
		{
			double weightNew = 1.0 / _hiddenLayer[selectedHiddenNeuronIndex]->age + 1;
			double weightOld = 1.0 - weightNew;
			
			*_hiddenLayer[selectedHiddenNeuronIndex]->currentOutputPart = 
				weightOld * *_hiddenLayer[selectedHiddenNeuronIndex]->currentOutputPart + 
				weightNew * *_outputLayer.currentOutput;
			*_hiddenLayer[selectedHiddenNeuronIndex]->currentOutputPart /= 
				_hiddenLayer[selectedHiddenNeuronIndex]->currentOutputPart->calculateNorm();
			
			*_hiddenLayer[selectedHiddenNeuronIndex]->nextOutputPart = 
				weightOld * *_hiddenLayer[selectedHiddenNeuronIndex]->nextOutputPart + 
				weightNew * output;
			*_hiddenLayer[selectedHiddenNeuronIndex]->nextOutputPart /= 
				_hiddenLayer[selectedHiddenNeuronIndex]->nextOutputPart->calculateNorm();
			
			*_hiddenLayer[selectedHiddenNeuronIndex]->inputPart = 
				weightOld * *_hiddenLayer[selectedHiddenNeuronIndex]->inputPart + 
				weightNew * *_inputLayer;
			*_hiddenLayer[selectedHiddenNeuronIndex]->inputPart /= 
				_hiddenLayer[selectedHiddenNeuronIndex]->inputPart->calculateNorm();
			
			++_hiddenLayer[selectedHiddenNeuronIndex]->age;
			++_hiddenLayer[selectedHiddenNeuronIndex]->frequency;
		}
		
		*_outputLayer.nextOutput = *_hiddenLayer[selectedHiddenNeuronIndex]->nextOutputPart;
	}
	else
	{
		unsigned selectedHiddenNeuronIndex = 0;
	
		double bestMatchResponse = -1;
		double currentResponse;
	
		for (unsigned currentIndex = 0; currentIndex < _hiddenLayer.size(); ++currentIndex)
		{
			currentResponse = _outputLayer.currentOutput->
				dot(*_hiddenLayer[currentIndex]->currentOutputPart);
		
			if (currentResponse > bestMatchResponse)
			{
				selectedHiddenNeuronIndex = currentIndex;
				bestMatchResponse = currentResponse;
			}
		}
		
		*_outputLayer.nextOutput = *_hiddenLayer[selectedHiddenNeuronIndex]->nextOutputPart;
	}
}

unsigned DevelopmentalNetwork::selectHiddenNeuron(bool isTraining)
{
	unsigned selectedHiddenNeuronIndex = 0;
	
	double bestMatchResponse = -1;
	double currentResponse;
	
	for (unsigned currentIndex = 0; currentIndex < _hiddenLayer.size(); ++currentIndex)
	{
		currentResponse = _outputLayer.currentOutput->
			dot(*_hiddenLayer[currentIndex]->currentOutputPart) + 
			_hiddenLayer[currentIndex]->inputPart->dot(*_inputLayer);
		
		if (currentResponse > bestMatchResponse)
		{
			selectedHiddenNeuronIndex = currentIndex;
			bestMatchResponse = currentResponse;
		}
	}
	
	if (isTraining && _hiddenLayer.size() < _hiddenNeuronCap && bestMatchResponse != 2)
	{
		selectedHiddenNeuronIndex = _hiddenNeuronCap;
	}
	
	return selectedHiddenNeuronIndex;
}
