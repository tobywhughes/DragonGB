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
	stackPointer = 0xFFFF;
	
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

	//RST
	else if ((opcode & 0xC7) == 0xC7)
	{
		unsigned char t = opcode & 0x38 >> 3;
		
		if(t == 0x0)
			programCounter = 0x0000;
		else if(t == 0x1)
			programCounter = 0x0008;
		else if(t == 0x2)
			programCounter = 0x0010;
		else if(t == 0x3)
			programCounter = 0x0018;
		else if(t == 0x4)
			programCounter = 0x0020;
		else if(t == 0x5)
			programCounter = 0x0028;
		else if(t == 0x6)
			programCounter = 0x0030;
		else if(t == 0x7)
			programCounter = 0x0038;
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

		stackPointer -= 1;
		memoryMap[stackPointer] = 0x00;
		stackPointer -= 1;
		memoryMap[stackPointer] = programCounter & 0x00FF;
		
	}

	//XOR & AND
	else if((opcode & 0xF0) == 0xA0)
	{
		//SET FLAGS
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
	//Nop
	else if(opcode == 0x00)
	{
		programCounter += 1;
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
