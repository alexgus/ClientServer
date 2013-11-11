/*
 * Main file of the SCP Project
 */

#include <iostream>
#include "Log.h"
using namespace std;

int main()
{
// Begin
	// Declaration
	Log log;


// Test Log
	log.write("Debug test", Log::DEBUG);
	log.write("Warning test", Log::WARN);
	log.write("Erreur test", Log::ERR);
	log << "Without level";

// Test display
	cout << "Hello World !" << endl;


// End
	return 0;
}
