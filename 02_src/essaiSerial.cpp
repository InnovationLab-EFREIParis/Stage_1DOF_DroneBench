#include "mbed.h"
#include <cstdio>
#include <iostream>
#include <cwchar>
#include <string>

#define RATE 200ms
using ThisThread::sleep_for;

static BufferedSerial pc(USBTX, USBRX, 9600);
 char choix;
// main() runs in its own thread in the OS
int main()
{
    //char choix;
    printf("Init\n");
    DigitalOut led(LED1);
   // sleep_for(1000ms);
    

    printf("Johny aime le feu?\n");
    printf("oui ou non?\n");
    
    choix = getchar();
   
    printf("ok");

    printf("%c \n",choix);
    

    
    while (true) {

        switch (choix) {
        case 'o':
        printf("Allumez le feu \n");
        led = !led;
        break;
        case 'n':
        printf("Inculte \n");
        break;
        }
        sleep_for(RATE);
    }
}

