#ifndef CPU_H
#define CPU_H
#define PIXELS 23040

typedef enum {false, true} bool;

unsigned char memoryMap[0xFFFF];
unsigned char width;
unsigned char height;
unsigned short screen[PIXELS];
unsigned short programCounter;
unsigned short AFReg;

bool loadGameToROM(char* fileName[]);
void initializeSystem();
void cycleCPU();

#endif
