#include <stdio.h>
#include "cpu.h"

//DRAGON GAME BOY EMULATOR

int main(int argc, char *argv[])
{
	//Checks if command line argument was inputed correctly
	if (argc != 2)
	{
		//Eventually will add more complicated command line handling
		//But for now, if it is not FILE GAME.GB then it is wrong
		printf("wat");
		return 0;
	}

	initializeSystem();

	//Loads file and exits if file failed
	bool loaded = loadGameToROM(&argv[1]);
	if (!loaded)
	{
		printf("File Load failed. Exiting");
		return 0;
	}


	//System Loop
	while(true)
	{
		cycleCPU();
		
		//if(drawFlag)
		//{
			//drawToScreen();
		//}

		//getInput()
	}

	return 0;
}


