//===---------__ Hacking STEM – EC_Meter.1.0.1.ino – Arduino __-----------===//
// For use with the "ELECTRICAL CONDUCTIVITY SENSOR Measuring Water Quality to
// Study Human Impact" lesson plan available from Microsoft Education Workshop
// at https://www.microsoft.com/en-us/education/education-workshop/conductivity-sensor.aspx
// http://aka.ms/hackingSTEM
//
// This project uses an Arduino UNO microcontroller board, information at:
// https://www.arduino.cc/en/main/arduinoBoardUno
//
// Overview:
// The Electrical Conductivity (EC) Meter are electrodes that emit and receive
// voltage through liquid, the measurements of this voltage is read and sent
// through Serial and read by Excel.
//
// Each reading is printed to serial as "voltage," for example:
// 1.1231,
//
// Comments, contributions, suggestions, bug reports, and feature requests
// are welcome! For source code and bug reports see:
// http://github.com/[TODO github path to Hacking STEM]
//
// Copyright 2018, Microsoft EDU Workshop - HackingSTEM
// MIT License terms detailed in LICENSE.txt
//===----------------------------------------------------------------------===//

// CONSTANTS
// Constants are variables that are only written to during initialization,
// we'll use these later in the code to refer to these values in an easier to
// read sort of way.
const byte    POWER_PIN       = 2;        // Pin for powering pulse circuit
const byte    EC_PIN          = A0;       // Analog pin used to read EC voltage
const int     READ_DURATION   = 100;      // Time to take reading for (ms)
const int     READ_INTERVAL   = 6000;     // Time between readings (ms)
const String  SERIAL_OUTPUT_DELIMITER   = ",";    // Formatting for output
const int     SERIAL_OUTPUT_INTERVAL    = 175;    // Time between serial writes

// These variables are used to track time intervals when reading from the EC pin
unsigned long currentTime     = 0;
unsigned long readStartTime   = 0; // Time that the EC Meter read process starts
unsigned long lastOutputTime  = 0; // Time of last serial output

// Variables used when reading values from EC pin
float ecMeterReading = 0;
float ecMeterVoltage = 0;

// setup() is run once
void setup() {
  Serial.begin( 9600 ); // Initialize Serial connection

  pinMode( POWER_PIN , OUTPUT ); // POWER_PIN as digital output
}

// loop() runs repeatedly, it is the heart beat of the program.
void loop() {
  currentTime = millis(); //set the currentTime to moment each time loop begins

  // If READ_INTERVAL has elapsed since last read,
  // then reset the read start time stamp.
  if ( currentTime - readStartTime >= READ_INTERVAL ){
    readStartTime = currentTime;

  // If READ_INTERVAL has NOT elapsed since last read,
  // AND READ_DURATION has NOT elapsed since read started,
  // then make sure power is on and take a(nother) reading
  } else if ( currentTime - READ_DURATION <= readStartTime ) {
    startPower( POWER_PIN );      // Start charging the solution
    delay(250);                   // Delay until solution is fully charged
    readSensors();                // Take sensor reading
    delay(250);                   // TODO delay for why?

  // If READ_INTERVAL has NOT elapsed since last read,
  // AND READ_DURATION HAS elapsed since read started,
  // then turn off power
  } else {
    stopPower( POWER_PIN );       // Stop charging and let solution depolarize
  }

  writeToSerial();
}

// Convert meter reading to voltage
void readSensors() {
  ecMeterReading = analogRead( EC_PIN );  // read the voltage from EC electrode
  ecMeterVoltage = ecMeterReading * 5.0;
  ecMeterVoltage = ecMeterVoltage / 1024;
}


//===----------------------------------------------------------------------===//
// Utility Methods Below Here
//===----------------------------------------------------------------------===//


// Turn on power to pin
void startPower( byte pin ){
  digitalWrite( pin , HIGH );
}

// Turn off power to pin
void stopPower ( byte pin ){
  digitalWrite( pin , LOW );
}

// Write ec values to serial if enough time has passed since last write
void writeToSerial() {
  // If at least SERIAL_OUTPUT_INTERVAL milliseconds have not elapsed
  // since last Serial write, return before doing anything
  if ( currentTime - lastOutputTime < SERIAL_OUTPUT_INTERVAL ) {
    return; // ends method before doing anything else.
  }

  Serial.print( ecMeterVoltage , 4 ); // Print ec voltage to 4 decimal places
  Serial.print( SERIAL_OUTPUT_DELIMITER ); // Print comma, expected by Excel
  Serial.println(); // End current line

  // set lastOutputTime to now, so we won't write to Serial again
  // until SERIAL_OUTPUT_INTERVAL milliseconds have elapsed
  lastOutputTime = currentTime;
}
