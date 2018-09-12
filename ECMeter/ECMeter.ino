/*
 * Electroconductivity Meter code for use with the Measuring Water to Understand Human Impact lesson plan 
 * Available from the Microsoft Education Workshop at http://aka.ms/hackingSTEM
 * 
 * This projects uses an Arduino UNO microcontroller board. More information can
 * be found by visiting the Arduino website: https://www.arduino.cc/en/main/arduinoBoardUno
 * 
 * The EC Meter raw ADC readings are converted to Volts and sent directly to Excel. 
 * 
 * 2017 Microsoft EDU Workshop
 * 
 */

// CONSTANTS / SETTINGS

const byte    POWER_PIN       = 2;        // Digital pin to use for powering pulse circuit
const byte    EC_PIN          = A0;       // Analog pin used to read EC Meter volage 

const int     READ_DURATION   = 100;      // Amount of time to take reading for (ms)
const int     READ_INTERVAL   = 6000;     // Amount of time between readings (ms)

const String  SERIAL_OUTPUT_DELIMITER   = ",";    // Cordoba expects a comma-delimited string
const int     SERIAL_OUTPUT_INTERVAL    = 175;    // Amount of time between serial outputs

// TIMESTAMPS

unsigned long currentTime     = 0;
unsigned long readStartTime   = 0;        // Time that the EC Meter read process starts
unsigned long lastOutputTime  = 0;        // Time of last serial output

// SENSORS

float ecMeterReading = 0;
float ecMeterVoltage = 0;

void setup() {
  Serial.begin( 9600 );
  
  pinMode( POWER_PIN , OUTPUT );
}

void loop() {
  currentTime = millis();

  // Read Sensors
  // Check for start read time
  if ( currentTime - readStartTime >= READ_INTERVAL ){          // Initialize EC Meter read sequence
    readStartTime = currentTime;                                // Set start time
  }  
  else if ( currentTime - READ_DURATION <= readStartTime )      // Power Up / Read
  {
    startPower( POWER_PIN );                                    // Start charging solution
    delay(250);                                                 // Delay until fully charged
    readSensors();                                              // Take sensor readings
    delay(250);
  } else {                                                      // Power Down
    stopPower( POWER_PIN );                                     // Stop charging and let solution depolarize
  }
  
  writeToSerial();
}

/*
  -------------------------------------------------------------------------------------------------------
    SENSOR/ARDUINO PROCESSING
  -------------------------------------------------------------------------------------------------------
*/

void startPower( byte pin ){
  digitalWrite( pin , HIGH );
}

void stopPower ( byte pin ){
  digitalWrite( pin , LOW );
}

void readSensors() {
  ecMeterReading = analogRead( EC_PIN );            // read the voltage from the EC Meter
  ecMeterVoltage = ecMeterReading * 5.0;
  ecMeterVoltage = ecMeterVoltage / 1024;
}

/*
  -------------------------------------------------------------------------------------------------------
    OUTGOING SERIAL DATA TO EXCEL PROCESSING CODE
  -------------------------------------------------------------------------------------------------------
*/

void writeToSerial()
{
  // Ignore if not within output interval
  if ( currentTime - lastOutputTime < SERIAL_OUTPUT_INTERVAL ) {
    return;
  }
  Serial.print( ecMeterVoltage , 4 );
  Serial.print( SERIAL_OUTPUT_DELIMITER );
  
  Serial.println();

  lastOutputTime = currentTime;
}


