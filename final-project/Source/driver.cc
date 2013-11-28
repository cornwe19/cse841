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
	if (argc != 11 && argc != 7)
	{
		cerr << "Bad parameters! Please see README.txt." << endl;
		
		return 1;
	}
	
	if (argc == 11)
	{
		unsigned hiddenNeuronCap = atoi(argv[5]);
		
		DevelopmentalNetwork developmentalNetwork(hiddenNeuronCap, argv[10]);
		developmentalNetwork.process(argv[3], true);
		developmentalNetwork.save(argv[8]);
	}
	else if (argc == 7)
	{
		DevelopmentalNetwork developmentalNetwork(100, argv[6]);
		developmentalNetwork.load(argv[4]);
		developmentalNetwork.process(argv[2], false);
	}
	
	return 0;
}
