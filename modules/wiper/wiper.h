#ifndef _WIPER_H_
#define _WIPER_H_

#include "mbed.h"
#include "arm_book_lib.h"

#include "user_interface.h"

#include "display.h"

#include "engine.h"

// Wiper physical parameters
#define WIPER_MAX_ANGLE         67.0f      // Maximum wiper angle (degrees)

// Cycle times (in seconds) for ramp motion:
#define HI_HALF_CYCLE_TIME      0.28f      // High speed: 0.28 s per half‑cycle.
#define LO_HALF_CYCLE_TIME      0.37f      // Low speed and INT: 0.37 s per half‑cycle.

// Angular speeds (degrees per second)
#define HI_ANGULAR_SPEED        (WIPER_MAX_ANGLE / HI_HALF_CYCLE_TIME)
#define LO_ANGULAR_SPEED        (WIPER_MAX_ANGLE / LO_HALF_CYCLE_TIME)

// Intermittent mode pause durations (in seconds)
#define INT_DELAY_SHORT         3.0f
#define INT_DELAY_MEDIUM        6.0f
#define INT_DELAY_LONG          8.0f

// Analog thresholds for wiper mode selection (0.0 to 1.0 readings).
#define MODE_OFF_THRESHOLD      0.25f  // reading < 0.25  => OFF.
#define MODE_HI_THRESHOLD       0.50f  // 0.25 <= reading < 0.50 => HI.
#define MODE_LO_THRESHOLD       0.75f  // 0.50 <= reading < 0.75 => LO.
                                       // reading >= 0.75 => INT.

enum WiperMode {
    WIPER_OFF,
    WIPER_HI,
    WIPER_LO,
    WIPER_INT
};

enum WiperCycleState {
    RAMP_UP,
    RAMP_DOWN,
    PAUSE,
    IDLE
};

void update_wiper_system();

#endif