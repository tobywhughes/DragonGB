#include <stdio.h>
#include <stdlib.h>

#define PIXELS 23040

//DRAGON GAME BOY EMULATOR

typedef enum {false, true} bool;

//Memory
unsigned char memoryMap[0xFFFF] = { 0 };

//Graphics
unsigned char width = 160;
unsigned char height = 144;
unsigned short screen[PIXELS] = { 0 };

//Proto
bool loadGameToROM(char* filename[])
{
	//Open file for reading
        FILE * gbFILE = fopen(*filename, "rb");
	if (gbFILE == NULL)
	{
		printf("Game file not loaded.");
		return false;
	}

	//Get filesize
	fseek(gbFILE, 0, SEEK_END);
	long fileSize = ftell(gbFILE);
	rewind(gbFILE);

	//Create buffer for file
	char * buffer = (char*)malloc(sizeof(char) * fileSize);
	if (buffer == NULL)
	{
		printf("Buffer error");
		return false;
	}

	//Read into buffer
	size_t read_length = fread(buffer, 1, fileSize, gbFILE);
	if (read_length != fileSize)
	{
		printf("Read error");
		return false;
	}

	if (fileSize <= 0x8000)
	{
		for(int i = 0; i < fileSize; ++i)
		{
			memoryMap[i] = buffer[i];	
		}
	}
	else
	{
		printf("%lx",fileSize);
		printf("ROM TOO LARGE");
		return false;	
	}
	fclose(gbFILE);
	free(buffer);
	return true;	
}      

int main(int argc, char *argv[])
{
	if (argc != 2)
	{
		//Eventually will add more complicated command line handling
		//But for now, if it is not FILE GAME.GB then it is wrong
		printf("wat");
		return 0;
	}
	bool loaded = loadGameToROM(&argv[1]);
	if (!loaded)
	{
		return 0;
	}

	return 0;
}


