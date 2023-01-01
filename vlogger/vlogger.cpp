// vlogger.cpp : This file contains the 'main' function. Program execution begins and ends there.
//

#include <iostream>
#include "vlogger.h"

void log_it(int id,int line ,const char* src_file) {
	std::string formattedString = std::format("ProcessId: {} Line {} File {}\n", id,line,src_file);
	Vlogger::Trace(formattedString);
}

int main()
{
	Vlogger::SetPriority(Vlogger::LogPriority::TracePriority);
	std::thread threads[10];
	for (int i = 0; i < 10; i++) {
		threads[i] = std::thread(log_it, i,__LINE__,__FILE__);
	}
	for (int i = 0; i < 10; i++)
		threads[i].join();
	Vlogger::EnableFileOutput("logs.txt");

	return 0;
}

// Run program: Ctrl + F5 or Debug > Start Without Debugging menu
// Debug program: F5 or Debug > Start Debugging menu

// Tips for Getting Started: 
//   1. Use the Solution Explorer window to add/manage files
//   2. Use the Team Explorer window to connect to source control
//   3. Use the Output window to see build output and other messages
//   4. Use the Error List window to view errors
//   5. Go to Project > Add New Item to create new code files, or Project > Add Existing Item to add existing code files to the project
//   6. In the future, to open this project again, go to File > Open > Project and select the .sln file
