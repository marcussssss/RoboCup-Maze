/**
* Author: Marcus 
* Date: 20/11/2019
* Aim: Testing IR sensor.

**/
#include <SharpDistSensor.h>

// Analog pin to which the sensor is connected
const byte sensorPin = A0;

// Window size of the median filter (odd number, 1 = no filtering)
const byte medianFilterWindowSize = 5;

// Create an object instance of the SharpDistSensor class
SharpDistSensor sensor(sensorPin, medianFilterWindowSize);

void setup() {
  Serial.begin(9600);

  // Set sensor model
  sensor.setModel(SharpDistSensor::GP2Y0A51SK0F_5V_DS);
}

void loop() {
  // Get distance from sensor
  unsigned int distance = sensor.getDist();

  // Print distance to Serial
  Serial.println(distance);

  // Wait some time
  delay(50);
}

//https://github.com/DrGFreeman/SharpDistSensor
// ir temp https://forum.arduino.cc/index.php?topic=210113.0
