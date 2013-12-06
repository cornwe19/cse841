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
   // Hardcode neruon cap to the static state table size for now
   unsigned hiddenNeuronCap = 60;
   
   DevelopmentalNetwork developmentalNetwork( hiddenNeuronCap );
   developmentalNetwork.process();
	
	return 0;
}
