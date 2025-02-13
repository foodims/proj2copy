#include "engine.h"

DigitalIn ignition(BUTTON1);
DigitalIn driver_occupancy_sensor(D15);
DigitalIn driver_seatbelt_sensor(D12);
DigitalIn passenger_occupancy_sensor(D14);
DigitalIn passenger_seatbelt_sensor(D13);

DigitalOut alarm_buzzer(PE_10);
DigitalOut greenLed(LED1);
DigitalOut blueLed(LED2);

UnbufferedSerial uartUsb(USBTX, USBRX, 115200);

EngineState engineState = engine_Off;
bool driverSitting = false;
bool ignitionPressed = false;

EngineState getEngineState() {
    return engineState;
}

//Input initilizations
void inputsInit() {
    ignition.mode(PullDown);
    driver_occupancy_sensor.mode(PullDown);
    driver_seatbelt_sensor.mode(PullDown);
    passenger_occupancy_sensor.mode(PullDown);
    passenger_seatbelt_sensor.mode(PullDown);
}

//Output initilizations
void outputsInit() {
    greenLed = OFF;
    blueLed = OFF;
    alarm_buzzer = ON;
}

//First system behavior
//Checks if driver is sitting down and displays welcome message
void check_driver_sit_down() {
    if (driver_occupancy_sensor && !driverSitting) {
        uartUsb.write("Welcome to an enhanced alarm system model 218-W25\r\n", 51);
        uartUsb.write("\r\n", 2);
        driverSitting = true;
    }
}

//Second system behavior
//Turns on green LED when both seat belts are fastened signifying a proper start is possible 
void check_both_seat_and_seatbelt() {
    if (driver_seatbelt_sensor && driver_occupancy_sensor && passenger_seatbelt_sensor && passenger_occupancy_sensor) {
        greenLed = ON;
    } 
    else {
        greenLed = OFF;
    }
}

void reset_system() {
    alarm_buzzer = ON; 
    blueLed = OFF;      
    greenLed = OFF;     
    driverSitting = false; 
    ignitionPressed = false; 
}

void process_fsm() {
switch (engineState) {

//First case (OFF)
    case engine_Off: 
        if (ignition && !ignitionPressed) {
            ignitionPressed = true;
            } else if (!ignition && ignitionPressed) {
                ignitionPressed = false;
                if (greenLed.read()) {
                    engineState = engine_Start;
                } else {
                    uartUsb.write("Ignition Inhibited. Ensure all conditions are met.\r\n", 52);
                    uartUsb.write("\r\n", 2);
                    alarm_buzzer = OFF;


                    if (!driver_occupancy_sensor) {
                        uartUsb.write("Driver Seat Not Occupied\r\n", 26);
                        uartUsb.write("\r\n", 2);
                    }
                    if (!passenger_occupancy_sensor) {
                        uartUsb.write("Passenger Seat Not Occupied\r\n", 29);
                        uartUsb.write("\r\n", 2);
                    }
                    if (!driver_seatbelt_sensor) {
                        uartUsb.write("Driver Seatbelt Not Fastened\r\n", 29);
                        uartUsb.write("\r\n", 2);
                    }
                    if (!passenger_seatbelt_sensor) {
                        uartUsb.write("Passenger Seatbelt Not Fastened\r\n", 33);
                        uartUsb.write("\r\n", 2);
                    }
                    engineState = engine_Off;
                    
                }
            }
            break;

//Second case (Start)
    case engine_Start:
        uartUsb.write("Engine started\r\n", 16);
        blueLed = ON;
        greenLed = OFF;
        alarm_buzzer = ON;
        engineState = engine_On;
        break;

//Third case (ON)
    case engine_On:
        if (ignition && !ignitionPressed) {
            ignitionPressed = true;
        }
        if (!ignition && ignitionPressed) {
            ignitionPressed = false;
            engineState = engine_Stop;
        }
        break;

//Fourth case (Stop)
    case engine_Stop:
        uartUsb.write("Engine stopped\r\n", 16);
        blueLed = OFF;
        greenLed = OFF;
        alarm_buzzer = OFF;
        reset_system();
        engineState = engine_Off;
        break;
    }
}