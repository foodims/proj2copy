//=====[Libraries]=============================================================

#include "mbed.h"
#include "arm_book_lib.h"

#include "user_interface.h"

#include "display.h"

//=====[Declaration of private defines]========================================

#define DISPLAY_REFRESH_TIME_MS 1000

//=====[Declaration of private data types]=====================================

//=====[Declaration and initialization of public global objects]===============

//=====[Declaration of external public global variables]=======================

//=====[Declaration and initialization of public global variables]=============


//=====[Declaration and initialization of private global variables]============

//=====[Declarations (prototypes) of private functions]========================

static void userInterfaceDisplayInit();
static void userInterfaceDisplayUpdate();

//=====[Implementations of public functions]===================================

/*void userInterfaceInit()
{
    userInterfaceDisplayInit();
}*/

void userInterfaceUpdate()
{
    userInterfaceDisplayUpdate();
}


//=====[Implementations of private functions]==================================

/*static void userInterfaceDisplayInit()
{
    displayInit();
     
    displayCharPositionWrite ( 0,0 );
    displayStringWrite( "Tmp:" );

    displayCharPositionWrite ( 9,0 );
    displayStringWrite( "Gas:" );
    
    displayCharPositionWrite ( 0,1 );
    displayStringWrite( "Alarm:" );
}*/

static void userInterfaceDisplayUpdate()
{

        displayCharPositionWrite ( 0,0 );
        displayStringWrite( "Welcome!" );;


}