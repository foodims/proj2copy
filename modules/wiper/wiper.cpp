#include "wiper.h"
#include "engine.h"

// ----- Windshield Wiper Subsystem ----- //
AnalogIn wiperModeSelector(A0);    // Potentiometer for wiper mode selection.
AnalogIn wiperDelaySelector(A2);   // Potentiometer for intermittent delay selection.

PwmOut wiperMotor(D8);  // Servo used to simulate the wiper motor.

// Wiper state variables.
WiperMode currentWiperMode = WIPER_OFF; // Current wiper mode.
WiperCycleState wiperState = IDLE; // Current wiper cycle state.
float currentAngle = 0.0f; // Current wiper angle (degrees).

// Variables for intermittent mode pause timing.
float pauseStartTime = 0.0f; // Time when the pause started.
float currentPauseDuration = 0.0f; // Current pause duration (seconds).

// Timer for non‑blocking wiper updates.
Timer wiperTimer; // Timer for non‑blocking wiper updates.
float lastWiperUpdate = 0.0f; // Time of the last wiper update (seconds).

void update_wiper_system() {
    // Read the desired wiper mode and (if INT) the delay time.
    float modeReading = wiperModeSelector.read();  // 0.0 to 1.0.
    WiperMode desiredMode;
    if (modeReading < MODE_OFF_THRESHOLD)
        desiredMode = WIPER_OFF;
    else if (modeReading < MODE_HI_THRESHOLD)
        desiredMode = WIPER_HI;
    else if (modeReading < MODE_LO_THRESHOLD)
        desiredMode = WIPER_LO;
    else
        desiredMode = WIPER_INT;
    
    // For INT mode, read the delay selector.
    float delayReading = wiperDelaySelector.read();
    float intDelay = INT_DELAY_SHORT;
    if (delayReading < 0.33f)
        intDelay = INT_DELAY_SHORT;
    else if (delayReading < 0.66f)
        intDelay = INT_DELAY_MEDIUM;
    else
        intDelay = INT_DELAY_LONG;
    
    // Update the LCD display with the current wiper mode (and delay if INT).
    switch(desiredMode) {
        case WIPER_OFF:
            displayWiperOff();
            break;
        case WIPER_HI:
            displayWiperHi();
            break;
        case WIPER_LO:
            displayWiperLo();
            break;
        case WIPER_INT:
            displayWiperInt();
            if (intDelay == INT_DELAY_SHORT)
                displayDelayShort();
            else if (intDelay == INT_DELAY_MEDIUM)
                displayDelayMedium();
            else if (intDelay == INT_DELAY_LONG)
                displayDelayLong();
            break;
    }
    
    // Only run the wipers if the engine is running.
    bool engineRunning = (getEngineState() == engine_On);
    if (!engineRunning) { // Engine is OFF.
        if (wiperState == IDLE) { // If the wipers are stopped, set the mode to OFF.
            currentAngle = 0.0f; // Reset the angle.
            wiperMotor.write(currentAngle); // Reset the motor.
            currentWiperMode = WIPER_OFF; // Set the mode to OFF.
        }
        return;
    }
    
    // If the engine is running and the desired mode is not OFF, update the active mode.
    if (desiredMode != WIPER_OFF) // If the desired mode is not OFF
        currentWiperMode = desiredMode; // Update the active mode.
    else if (wiperState == IDLE) // If the wipers are stopped, set the mode to OFF.
        currentWiperMode = WIPER_OFF; // Set the mode to OFF.
    
    // Run the wiper cycle state machine.
    float now = (float)wiperTimer.elapsed_time().count() / 1e9f; // Convert nanoseconds to seconds.
    float dt = now - lastWiperUpdate; // Time change since last update.
    lastWiperUpdate = now; // Update the last update time.
    
    // Determine angular speed based on mode.
    float angularSpeed = 0.0f; 
    if (currentWiperMode == WIPER_HI)
        angularSpeed = HI_ANGULAR_SPEED;
    else if (currentWiperMode == WIPER_LO || currentWiperMode == WIPER_INT)
        angularSpeed = LO_ANGULAR_SPEED;
    
    switch(wiperState) {
        case IDLE: // Wipers are stopped. So reset the angle.
            if (currentWiperMode == WIPER_HI ||
                currentWiperMode == WIPER_LO ||
                currentWiperMode == WIPER_INT)
            {
                wiperState = RAMP_UP;
                currentAngle = 0.0f;
            }
            break;
            
        case RAMP_UP: // Wipers are moving up.
            currentAngle += angularSpeed * dt; // Increment the angle by angularSpeed * dt.
            if (currentAngle >= WIPER_MAX_ANGLE) {
                currentAngle = WIPER_MAX_ANGLE;
                wiperState = RAMP_DOWN;
            }
            break;
            
        case RAMP_DOWN:
            currentAngle -= angularSpeed * dt; // Decrement the angle by angularSpeed * dt.
            if (currentAngle <= 0.0f) {
                currentAngle = 0.0f;
                if (currentWiperMode == WIPER_INT) { // If in INT mode, pause before the next cycle.
                    wiperState = PAUSE;
                    pauseStartTime = now;
                    currentPauseDuration = intDelay - (intDelay * 2.0f); // Adjust for the time taken to complete the cycle.
                } else {
                    // If the user has switched to OFF, complete the cycle.
                    if (wiperModeSelector.read() < MODE_OFF_THRESHOLD)
                        wiperState = IDLE;
                    else
                        wiperState = RAMP_UP;
                }
            }
            break;
            
        case PAUSE:
            if ((now - pauseStartTime) >= currentPauseDuration) { // Pause is over.
                if (wiperModeSelector.read() < MODE_OFF_THRESHOLD) // If the user has switched to OFF, complete the cycle.
                    wiperState = IDLE;
                else
                    wiperState = RAMP_UP; // Start the next cycle.
            }
            break;
    }
    
    // Update the wiper motor (servo) output based on currentAngle.
    float duty = currentAngle / WIPER_MAX_ANGLE;
    wiperMotor.write(duty);

    if(getEngineState() == engine_Stop) {
        // Reset wiper motor position to 0°.
        currentAngle = 0.0f;
        wiperMotor.write(0.0f);
        
        wiperState = IDLE;
        currentWiperMode = WIPER_OFF;
    }
}