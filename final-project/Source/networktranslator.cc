/**
 * @file    networktranslator.cc
 * @author  Austin Gregory
 * @created 2013-11-15
 */

#include "networktranslator.hpp"

NetworkTranslator::NetworkTranslator()
{
	_inputs.push_back("_");
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

void NetworkTranslator::printInputs() const
{
	for (unsigned i = 0; i < _inputs.size(); ++i)
	{
		cout << _inputs[i] << endl;
	}
	
	cout << endl;
}

void NetworkTranslator::printOutputs() const
{
	for (unsigned i = 0; i < _outputs.size(); ++i)
	{
		cout << _outputs[i] << endl;
	}
	
	cout << endl;
}





void NetworkTranslator::addInputIfNotFound(string input)
{
	bool found = false;
	
	for (unsigned i = 0; i < _inputs.size(); ++i)
	{
		if (_inputs[i] == input)
		{
			found = true;
			break;
		}
	}
	
	if (!found)
	{
		_inputs.push_back(input);
	}
}

void NetworkTranslator::addOutputIfNotFound(string output)
{
	bool found = false;
	
	for (unsigned i = 0; i < _outputs.size(); ++i)
	{
		if (_outputs[i] == output)
		{
			found = true;
			break;
		}
	}
	
	if (!found)
	{
		_outputs.push_back(output);
	}
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

string NetworkTranslator::translateInput(Vector input)
{
	unsigned inputIndex = input.getSize() - 1;
	unsigned translatedIndex = unsigned(input.at(inputIndex));
	
	--inputIndex;
	unsigned power = 1;
	
	while (inputIndex > 0)
	{
		translatedIndex += unsigned(input.at(inputIndex)) << power;
		
		--inputIndex;
		++power;
	}
	
	// Need to convert the final bit.
	translatedIndex += unsigned(input.at(inputIndex)) << power;
	
	if (translatedIndex >= _inputs.size())
	{
		cerr << "\n\nException: Translating the input vector yields too large of an index.\n";
		cerr << "Input Vector: " << input << endl;
		cerr << "Translated Index: " << translatedIndex << endl << endl;
	}
	
	return _inputs[translatedIndex];
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
	
	if (translatedIndex >= _inputs.size())
	{
		cerr << "\n\nException: Translating the input vector yields too large of an index.\n";
		cerr << "Input Vector: " << *input << endl;
		cerr << "Translated Index: " << translatedIndex << endl << endl;
	}
	
	return _inputs[translatedIndex];
}





Vector NetworkTranslator::translateOutput(string output)
{
	Vector translatedOutput(_NUM_OUT_NEURONS);
	
	unsigned outputIndex = findOutputIndex(output);
	
	translatedOutput.at(outputIndex) = 1;
	
	return translatedOutput;
}

string NetworkTranslator::translateOutput(Vector output)
{
	unsigned outputIndex;
	
	for (outputIndex = 0; outputIndex < _NUM_OUT_POSSIBLE; ++outputIndex)
	{
		if (output.at(outputIndex) == 1)
		{
			break;
		}
	}
	
	if (outputIndex >= _NUM_OUT_POSSIBLE || output.calculateNorm() != 1)
	{
		cerr << "\n\nException: Translating the output vector failed.\n";
		cerr << "Output Vector: " << output << endl << endl;
	}
	
	return _outputs[outputIndex];
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
	
	return _outputs[outputIndex];
}





Vector NetworkTranslator::findNearestActualInput(Vector input)
{
	unsigned bestMatchIndex = 0;
	double bestMatchDistance = -1;
	
	double currentDistance;
	
	for (unsigned currentIndex = 0; currentIndex < _inputs.size(); ++currentIndex)
	{
		currentDistance = input.calculateDistanceTo(translateInput(_inputs[currentIndex]));
		
		if (bestMatchDistance < 0 || currentDistance < bestMatchDistance)
		{
			bestMatchIndex = currentIndex;
			bestMatchDistance = currentDistance;
		}
	}
	
	return translateInput(_inputs[bestMatchIndex]);
}

Vector NetworkTranslator::findNearestActualInput(Vector* input)
{
	unsigned bestMatchIndex = 0;
	double bestMatchDistance = -1;
	
	double currentDistance;
	
	for (unsigned currentIndex = 0; currentIndex < _inputs.size(); ++currentIndex)
	{
		currentDistance = input->calculateDistanceTo(translateInput(_inputs[currentIndex]));
		
		if (bestMatchDistance < 0 || currentDistance < bestMatchDistance)
		{
			bestMatchIndex = currentIndex;
			bestMatchDistance = currentDistance;
		}
	}
	
	return translateInput(_inputs[bestMatchIndex]);
}





Vector NetworkTranslator::findNearestActualOutput(Vector output)
{
	unsigned bestMatchIndex = 0;
	double bestMatchDistance = -1;
	
	double currentDistance;
	
	for (unsigned currentIndex = 0; currentIndex < _outputs.size(); ++currentIndex)
	{
		currentDistance = output.calculateDistanceTo(translateOutput(_outputs[currentIndex]));
		
		if (bestMatchDistance < 0 || currentDistance < bestMatchDistance)
		{
			bestMatchIndex = currentIndex;
			bestMatchDistance = currentDistance;
		}
	}
	
	return translateOutput(_outputs[bestMatchIndex]);
}

Vector NetworkTranslator::findNearestActualOutput(Vector* output)
{
	unsigned bestMatchIndex = 0;
	double bestMatchDistance = -1;
	
	double currentDistance;
	
	for (unsigned currentIndex = 0; currentIndex < _outputs.size(); ++currentIndex)
	{
		currentDistance = output->calculateDistanceTo(translateOutput(_outputs[currentIndex]));
		
		if (bestMatchDistance < 0 || currentDistance < bestMatchDistance)
		{
			bestMatchIndex = currentIndex;
			bestMatchDistance = currentDistance;
		}
	}
	
	return translateOutput(_outputs[bestMatchIndex]);
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
	
	for (inputIndex = 0; inputIndex < _inputs.size(); ++inputIndex)
	{
		if (_inputs[inputIndex] == input)
		{
			break;
		}
	}
	
	if (inputIndex >= _inputs.size())
	{
		cerr << "\n\nException: Failed to find input index.\n";
		cerr << "Input: " << input << endl << endl;
	}
	
	return inputIndex;
}

unsigned NetworkTranslator::findOutputIndex(string output)
{
	unsigned outputIndex;
	
	for (outputIndex = 0; outputIndex < _outputs.size(); ++outputIndex)
	{
		if (_outputs[outputIndex] == output)
		{
			break;
		}
	}
	
	if (outputIndex >= _outputs.size())
	{
		cerr << "\n\nException: Failed to find output index.\n";
		cerr << "Output: " << output << endl << endl;
	}
	
	return outputIndex;
}
