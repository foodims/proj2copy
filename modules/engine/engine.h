#ifndef _ENGINE_H_
#define _ENGINE_H_

#include "mbed.h"
#include "arm_book_lib.h"

enum EngineState {
    engine_Off,
    engine_Start,
    engine_On,
    engine_Stop
};

void inputsInit();
void outputsInit();
void check_driver_sit_down();
void check_both_seat_and_seatbelt();
void process_fsm();
void reset_system();
EngineState getEngineState();

#endif