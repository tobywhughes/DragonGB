#include <stdio.h>
#include <stdlib.h>
#include "cpu.h"

void initializeSystem()
{
	//Memory
	for(int i = 0; i < (sizeof(memoryMap)/sizeof(unsigned char)); i++)
	{
		memoryMap[i] = 0;
	}
	programCounter = 0x0000;
	AFReg = 0;
	
	//Graphics
	width = 160;
	height = 144;
	for(int i = 0; i < (sizeof(screen)/sizeof(unsigned char)); i++)
	{
		screen[i] = 0;
	}
}

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

	//Check if filesize is acceptable
	if (fileSize <= 0x8000)
        {
		//Read buffer into memory
                for(int i = 0; i < fileSize; ++i)
                {
                        memoryMap[i] = buffer[i];
                }
        }
	//ROM is too large
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


void cycleCPU()
{
	//Single byte opcode
	unsigned char opcode = memoryMap[programCounter];

	//JMP
	if(opcode == 0xC3)
	{
		programCounter = memoryMap[programCounter + 1] | (memoryMap[programCounter + 2] << 8);
	}

	//XOR & AND
	else if((opcode & 0xF0) == 0xA0)
	{
		//XOR Accumulator
		if (opcode == 0xAF)
		{
			AFReg = ((((AFReg & 0xF0) >> 8) ^ memoryMap[programCounter + 1]) << 8 ) | (AFReg & 0xF);
			programCounter += 2;
		}

		//Unknown Opcode
		else
		{
			printf("%x \n", opcode);
			printf("%x \n", memoryMap[programCounter + 1]);
			printf("%x \n", memoryMap[programCounter + 2]);
			while(true)
			{
				;
			}
		}
	}
	//Unknown opcode
	else
	{
		printf("%x \n", opcode);
		printf("%x \n", memoryMap[programCounter + 1]);
		printf("%x \n", memoryMap[programCounter + 2]);
		while(true)
		{
			;
		}
	}

}
