// Threading.cpp : Defines the entry point for the console application.
//

#include "stdafx.h"
#include "Clock.h"

bool stopSimulation = false;
bool newFrame = false;

void simulation(int* i)
{
	Clock timer;
	while (!stopSimulation)
	{
		if (timer.elapsedS() > 1.0)
		{
			if (newFrame = false)
			{
				i++; newFrame = true;
			}
		}
	}
}



const int nThreads = 10;

int main(int nArgsPlus1, char* args[])
{
	try
	{
		int output = 0;
		std::thread t(simulation, &output);
		while (output < 10)
		{
			if (newFrame == true)
			{
				std::cout << "output = " << output << std::endl;
				newFrame = false;
			}
		}
		stopSimulation = true;
		t.join();

		return 0;
	}
	catch (const std::exception& err)
	{
		std::cout << "In main:\n";
		std::cout << err.what() << "\n";
		return 1;
	}
}

