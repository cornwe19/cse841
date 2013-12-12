/**
 * @file driver.cc
 * @author Austin Gregory
 * @created 2013-10-06
 */

#include <cstdio>

#include "developmentalnetwork.hpp"
#include "networktranslator.hpp"

using namespace std;

int main(int argc, char* argv[])
{
	if (argc != 2)
	{
		cerr << "\nImproper arguments! Expecting starting state.\n\n";
		
		return 1;
	}
	
	// Hard code neuron cap to be sufficiently high.
	unsigned hiddenNeuronCap = 1000;
   
	DevelopmentalNetwork developmentalNetwork( argv[1], hiddenNeuronCap );
	developmentalNetwork.process();
	
	return 0;
}
