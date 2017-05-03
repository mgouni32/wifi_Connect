#include <wiringPi.h>
#include <stdio.h>
#include <time.h>

#define RED_PIN 3
#define GREEN_PIN 4
#define BLUE_PIN 5

void setColor(int red, int green, int blue);

int main (void)
{
    struct timespec tim, tim2;
    tim.tv_sec = 0;
    tim.tv_nsec = 500000000L;
    
    wiringPiSetup();
    pinMode(GREEN_PIN, OUTPUT);
    pullUpDnControl(GREEN_PIN, PUD_UP);
    pinMode(RED_PIN, OUTPUT);
    pullUpDnControl(RED_PIN, PUD_UP);
    pinMode(BLUE_PIN, OUTPUT);
    pullUpDnControl(BLUE_PIN, PUD_UP);
    
    for(;;){
        setColor(1,0,0);
        //nanosleep(&tim, &tim2);
        //setColor(0,255,0);
        //nanosleep(&tim, &tim2);
        //setColor(0,0,255);
        //nanosleep(&tim, &tim2);
        sleep(5);
    }
    
    return 0;
}

void setColor(int red, int green, int blue)
{
    //red = 255 - red;
    //green = 255 - green;
    //blue = 255 - blue;
    red = 1 - red;
    green = 1 - green;
    blue = 1 - blue;
    digitalWrite(RED_PIN, red);
    digitalWrite(GREEN_PIN, green);
    digitalWrite(BLUE_PIN, blue);
}
