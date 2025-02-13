//=====[Libraries]=============================================================

#include "mbed.h"
#include "arm_book_lib.h"

#include "user_interface.h"

#include "display.h"

//=====[Declaration of private defines]========================================

#define DISPLAY_REFRESH_TIME_MS 1000

void displayWiperOff() 
{
    displayCharPositionWrite(0, 0);
    displayStringWrite("Wiper: OFF");
}

void displayWiperHi()
{
    displayCharPositionWrite(0, 0);
    displayStringWrite("Wiper: HI");
}

void displayWiperLo()
{
    displayCharPositionWrite(0, 0);
    displayStringWrite("Wiper: LO");
}

void displayWiperInt() 
{
    displayCharPositionWrite(0, 0);
    displayStringWrite("Wiper: INT");
}

void displayDelayShort()
{
    displayCharPositionWrite(0, 1);
    displayStringWrite("Delay: SHORT");
}

void displayDelayMedium()
{
    displayCharPositionWrite(0, 1);
    displayStringWrite("Delay: MEDIUM");
}

void displayDelayLong()
{
    displayCharPositionWrite(0, 1);
    displayStringWrite("Delay: LONG");
}