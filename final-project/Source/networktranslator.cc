/**
 * @file    networktranslator.cc
 * @author  Austin Gregory
 * @created 2013-11-15
 */

#include "networktranslator.hpp"

const string NetworkTranslator::_INPUTS[NetworkTranslator::_NUM_IN_POSSIBLE] = 
	{
		"_", 
		"cat", "full", "hungry", "kitten", "looks", 
		"meal", "stares", "time", "well", "young"
	};

const string NetworkTranslator::_OUTPUTS[NetworkTranslator::_NUM_OUT_POSSIBLE] = 
	{
		"z1", "z2", "z3", 
		"z4", "z5", "z6"
	};

// Entry [output index][input index - 1] is the output index for the next state.
const unsigned NetworkTranslator::_IN_OUT_MAP[NetworkTranslator::_NUM_OUT_POSSIBLE]
	[NetworkTranslator::_NUM_IN_POSSIBLE - 1] = 
	{
		{0, 0, 5, 2, 0, 4, 0, 0, 0, 1},
		{2, 0, 5, 2, 0, 4, 0, 0, 1, 1},
		{0, 0, 5, 2, 3, 4, 3, 0, 2, 1},
		{0, 0, 5, 2, 0, 4, 0, 0, 3, 1},
		{0, 0, 5, 2, 0, 4, 0, 5, 4, 1},
		{0, 0, 5, 2, 0, 4, 0, 0, 5, 1}
	};





NetworkTranslator::NetworkTranslator()
{
}

NetworkTranslator::~NetworkTranslator()
{
}





unsigned NetworkTranslator::getNumberOfInputNeurons() const
{
	return _NUM_IN_NEURONS;
}

unsigned NetworkTranslator::getNumberOfOutputNeurons() const
{
	return _NUM_OUT_NEURONS;
}





Vector NetworkTranslator::translateInput(string input)
{
	Vector translatedInput(_NUM_IN_NEURONS);
	
	unsigned inputIndex = findInputIndex(input);
	unsigned currentTranslatedIndex = translatedInput.getSize() - 1;
	
	while (inputIndex > 1 && currentTranslatedIndex > 0)
	{
		translatedInput.at(currentTranslatedIndex) = inputIndex & 1;
		
		inputIndex = inputIndex >> 1;
		--currentTranslatedIndex;
	}
	
	// Need to grab the final bit.
	translatedInput.at(currentTranslatedIndex) = inputIndex & 1;
	
	return translatedInput;
}

string NetworkTranslator::translateInput(Vector* input)
{
	unsigned inputIndex = input->getSize() - 1;
	unsigned translatedIndex = unsigned(input->at(inputIndex));
	
	--inputIndex;
	unsigned power = 1;
	
	while (inputIndex > 0)
	{
		translatedIndex += unsigned(input->at(inputIndex)) << power;
		
		--inputIndex;
		++power;
	}
	
	// Need to convert the final bit.
	translatedIndex += unsigned(input->at(inputIndex)) << power;
	
	if (translatedIndex >= _NUM_IN_POSSIBLE)
	{
		cerr << "\n\nException: Translating the input vector yields too large of an index.\n";
		cerr << "Input Vector: " << *input << endl;
		cerr << "Translated Index: " << translatedIndex << endl << endl;
	}
	
	return _INPUTS[translatedIndex];
}

Vector NetworkTranslator::translateOutput(string output)
{
	Vector translatedOutput(_NUM_OUT_NEURONS);
	
	unsigned outputIndex = findOutputIndex(output);
	
	translatedOutput.at(outputIndex) = 1;
	
	return translatedOutput;
}

string NetworkTranslator::translateOutput(Vector* output)
{
	unsigned outputIndex;
	
	for (outputIndex = 0; outputIndex < _NUM_OUT_POSSIBLE; ++outputIndex)
	{
		if (output->at(outputIndex) == 1)
		{
			break;
		}
	}
	
	if (outputIndex >= _NUM_OUT_POSSIBLE || output->calculateNorm() != 1)
	{
		cerr << "\n\nException: Translating the output vector failed.\n";
		cerr << "Output Vector: " << *output << endl << endl;
	}
	
	return _OUTPUTS[outputIndex];
}

Vector NetworkTranslator::findNextOutput(Vector* currentOutput, Vector* input)
{
	Vector* nearestOutput = new Vector(findNearestActualOutput(currentOutput));
	Vector* nearestInput = new Vector(findNearestActualInput(input));
	
	unsigned currentOutputIndex = findOutputIndex(translateOutput(nearestOutput));
	unsigned inputIndex = findInputIndex(translateInput(nearestInput));
	
	delete nearestOutput;
	delete nearestInput;
	
	// Need to use inputIndex - 1 because the _ is a special case and isn't in the map.
	return translateOutput(_OUTPUTS[_IN_OUT_MAP[currentOutputIndex][inputIndex - 1]]);
}

Vector NetworkTranslator::findNearestActualInput(Vector* input)
{
	unsigned bestMatchIndex = 0;
	double bestMatchDistance = -1;
	
	double currentDistance;
	
	for (unsigned currentIndex = 0; currentIndex < _NUM_IN_POSSIBLE; ++currentIndex)
	{
		currentDistance = input->calculateDistanceTo(translateInput(_INPUTS[currentIndex]));
		
		if (bestMatchDistance < 0 || currentDistance < bestMatchDistance)
		{
			bestMatchIndex = currentIndex;
			bestMatchDistance = currentDistance;
		}
	}
	
	return translateInput(_INPUTS[bestMatchIndex]);
}

Vector NetworkTranslator::findNearestActualOutput(Vector* output)
{
	unsigned bestMatchIndex = 0;
	double bestMatchDistance = -1;
	
	double currentDistance;
	
	for (unsigned currentIndex = 0; currentIndex < _NUM_OUT_POSSIBLE; ++currentIndex)
	{
		currentDistance = output->calculateDistanceTo(translateOutput(_OUTPUTS[currentIndex]));
		
		if (bestMatchDistance < 0 || currentDistance < bestMatchDistance)
		{
			bestMatchIndex = currentIndex;
			bestMatchDistance = currentDistance;
		}
	}
	
	return translateOutput(_OUTPUTS[bestMatchIndex]);
}





NetworkTranslator::NetworkTranslator(const NetworkTranslator& networkTranslator)
{
}





NetworkTranslator& NetworkTranslator::operator=(const NetworkTranslator& networkTranslator)
{
	return *this;
}





unsigned NetworkTranslator::findInputIndex(string input)
{
	unsigned inputIndex;
	
	for (inputIndex = 0; inputIndex < _NUM_IN_POSSIBLE; ++inputIndex)
	{
		if (_INPUTS[inputIndex] == input)
		{
			break;
		}
	}
	
	if (inputIndex >= _NUM_IN_POSSIBLE)
	{
		cerr << "\n\nException: Failed to find input index.\n";
		cerr << "Input: " << input << endl << endl;
	}
	
	return inputIndex;
}

unsigned NetworkTranslator::findOutputIndex(string output)
{
	unsigned outputIndex;
	
	for (outputIndex = 0; outputIndex < _NUM_OUT_POSSIBLE; ++outputIndex)
	{
		if (_OUTPUTS[outputIndex] == output)
		{
			break;
		}
	}
	
	if (outputIndex >= _NUM_OUT_POSSIBLE)
	{
		cerr << "\n\nException: Failed to find output index.\n";
		cerr << "Output: " << output << endl << endl;
	}
	
	return outputIndex;
}
