/**
 * @file    developmentalnetwork.cc
 * @author  Austin Gregory
 * @created 2013-10-30
 */

#include "developmentalnetwork.hpp"

DevelopmentalNetwork::DevelopmentalNetwork(unsigned hiddenNeuronCap, string outputFileStem)
{
	Vector normedStartingOutput(_translator.translateOutput("z1"));
	normedStartingOutput /= normedStartingOutput.calculateNorm();
	
	_outputLayer.currentOutput = new Vector(normedStartingOutput);
	_outputLayer.nextOutput = new Vector(_translator.getNumberOfOutputNeurons());
	
	_inputLayer = new Vector(_translator.getNumberOfInputNeurons());
	
	_hiddenNeuronCap = hiddenNeuronCap;
	_outputFileStem = outputFileStem;
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





void DevelopmentalNetwork::process(string sourceFileName, bool isTraining)
{
	ifstream sourceFile(sourceFileName.c_str());
	
	vector<string> words;
	string line;
	string word;

	while(getline(sourceFile, line))
	{
		word = "";
		
		for (unsigned i = 0; i < line.length(); ++i)
		{
			if (line[i] == ' ')
			{
				if (word != "")
				{
					words.push_back(word);
				
					word = "";
				}
			}
			else
			{
				word += line[i];
			}
		}
		
		if (word != "")
		{
			words.push_back(word);
		}
	}
	
	sourceFile.close();
	
	for (unsigned i = 0; i < words.size(); ++i)
	{
		cout << "Processing word: " << words[i] << endl;
		
		processInput(words[i], isTraining);
		
		Vector* nearestCurrentOutput = new Vector(
			_translator.findNearestActualOutput(_outputLayer.currentOutput));
		Vector* nearestNextOutput = new Vector(
			_translator.findNearestActualOutput(_outputLayer.nextOutput));
			
		cout << "Transition: " << _translator.translateOutput(nearestCurrentOutput);
		cout << " -> " << _translator.translateOutput(nearestNextOutput);
		cout << endl << endl;
		
		delete nearestCurrentOutput;
		delete nearestNextOutput;
		
		*_outputLayer.currentOutput = *_outputLayer.nextOutput;
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





void DevelopmentalNetwork::processInput(string word, bool isTraining)
{
	*_inputLayer = _translator.translateInput(word);
	
	if (_inputLayer->calculateNorm() > 0)
	{
		*_inputLayer /= _inputLayer->calculateNorm();
		
		unsigned selectedHiddenNeuronIndex = selectHiddenNeuron(isTraining);
		bool createdNewNeuron = false;
		
		if (selectedHiddenNeuronIndex == _hiddenNeuronCap)
		{
			HiddenNeuron* newNeuron = new HiddenNeuron();
			newNeuron->currentOutputPart = new Vector(*_outputLayer.currentOutput);
			newNeuron->inputPart = new Vector(*_inputLayer);
			
			newNeuron->nextOutputPart = 
				new Vector(_translator.findNextOutput(newNeuron->currentOutputPart, 
				newNeuron->inputPart));
			*newNeuron->nextOutputPart /= newNeuron->nextOutputPart->calculateNorm();
			
			newNeuron->age = 1;
		
			_hiddenLayer.push_back(newNeuron);
		
			selectedHiddenNeuronIndex = _hiddenLayer.size() - 1;
			createdNewNeuron = true;
		}
		
		if (isTraining && !createdNewNeuron)
		{
			double weightNew = 1.0 / _hiddenLayer[selectedHiddenNeuronIndex]->age + 1;
			double weightOld = 1.0 - weightNew;
			
			Vector normedNextOutput(_translator.findNextOutput(_outputLayer.currentOutput, 
				_inputLayer));
			normedNextOutput /= normedNextOutput.calculateNorm();
			
			*_hiddenLayer[selectedHiddenNeuronIndex]->currentOutputPart = 
				weightOld * *_hiddenLayer[selectedHiddenNeuronIndex]->currentOutputPart + 
				weightNew * *_outputLayer.currentOutput;
			*_hiddenLayer[selectedHiddenNeuronIndex]->currentOutputPart /= 
				_hiddenLayer[selectedHiddenNeuronIndex]->currentOutputPart->calculateNorm();
			
			*_hiddenLayer[selectedHiddenNeuronIndex]->nextOutputPart = 
				weightOld * *_hiddenLayer[selectedHiddenNeuronIndex]->nextOutputPart + 
				weightNew * normedNextOutput;
			*_hiddenLayer[selectedHiddenNeuronIndex]->nextOutputPart /= 
				_hiddenLayer[selectedHiddenNeuronIndex]->nextOutputPart->calculateNorm();
			
			*_hiddenLayer[selectedHiddenNeuronIndex]->inputPart = 
				weightOld * *_hiddenLayer[selectedHiddenNeuronIndex]->inputPart + 
				weightNew * *_inputLayer;
			*_hiddenLayer[selectedHiddenNeuronIndex]->inputPart /= 
				_hiddenLayer[selectedHiddenNeuronIndex]->inputPart->calculateNorm();
			
			++_hiddenLayer[selectedHiddenNeuronIndex]->age;
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
		
		Vector* nearestInput = new Vector(
			_translator.findNearestActualInput(_hiddenLayer[selectedHiddenNeuronIndex]->
			inputPart));
		
		cout << "Predicted Word: " << _translator.translateInput(nearestInput) << endl;
		
		delete nearestInput;
		
		*_outputLayer.nextOutput = *_hiddenLayer[selectedHiddenNeuronIndex]->nextOutputPart;
	}
}

unsigned DevelopmentalNetwork::selectHiddenNeuron(bool isTraining)
{
	unsigned selectedHiddenNeuronIndex = 0;
	
	Vector normedInput(*_inputLayer);
	normedInput /= normedInput.calculateNorm();
	
	double bestMatchResponse = -1;
	double currentResponse;
	
	for (unsigned currentIndex = 0; currentIndex < _hiddenLayer.size(); ++currentIndex)
	{
		currentResponse = _outputLayer.currentOutput->
			dot(*_hiddenLayer[currentIndex]->currentOutputPart) + 
			_hiddenLayer[currentIndex]->inputPart->dot(normedInput);
		
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
