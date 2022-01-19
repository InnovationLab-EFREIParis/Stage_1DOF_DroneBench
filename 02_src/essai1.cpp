#include "mbed.h"
#include <cstdio>

#define Rate 1000ms
static BufferedSerial pc(USBTX,USBRX,9600);


// main() runs in its own thread in the OS
int main()
{
   // char eh[]="Allumez le feu\n \r  ";
    while (true) {
      ThisThread::sleep_for(1000ms);
       //pc.write(eh, sizeof(eh));
       printf("Allumez le feu \n");
    }
}

