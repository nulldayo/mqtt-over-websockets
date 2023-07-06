#include "Arduino_BHY2.h"
#include "Nicla_System.h"
#include <ArduinoBLE.h>

BLEService environmentService("181A");
BLEFloatCharacteristic tempCharacteristic("2A1C", BLERead | BLENotify);
BLEFloatCharacteristic pressureCharacteristic("2A6D", BLERead | BLENotify);

// Advertising parameters should have a global scope. Do NOT define them in 'setup' or in 'loop'
float previousTemperature = 0;
float previousPressure = 0;
long previousMillis = 0;
const unsigned int UPDATE_FREQUENCY = 2000; // in ms

// Nicla Sense Sensors
SensorXYZ accel(SENSOR_ID_ACC_PASS);
SensorXYZ gyro(SENSOR_ID_GYRO_PASS);
Sensor temperature(SENSOR_ID_TEMP);
Sensor pressure(SENSOR_ID_BARO);
SensorBSEC bsec(SENSOR_ID_BSEC);

void setup() {
  Serial.begin(9600);
  // while (!Serial); // only when connected to laptop
  nicla::begin();
  nicla::leds.begin();

  BHY2.begin();
  accel.begin();
  gyro.begin();
  temperature.begin();
  pressure.begin();
  bsec.begin();
  
  if (!BLE.begin()) {
    Serial.println("failed to initialize BLE!");
    while (1);
  }

  BLE.setLocalName("NiclaSenseME"); // set name for connection
  BLE.setAdvertisedService("environmentService"); // advertise environment service
  
  environmentService.addCharacteristic(tempCharacteristic);
  environmentService.addCharacteristic(pressureCharacteristic);

  BLE.addService(environmentService);
  
  tempCharacteristic.setValue(0);        // Set initial temperature value
  pressureCharacteristic.setValue(0);    // Set initial pressure value

  BLE.advertise();
  nicla::leds.setColor(red);
  Serial.print("Peripheral device MAC: " + BLE.address() + "\n Waiting for connections...");
}

void loop() {
  BLEDevice central = BLE.central(); // Wait for a BLE central to connect

  // If central is connected to peripheral
  if (central) {
      Serial.print("Connected to central MAC: ");
      Serial.println(central.address()); // Central's BT address:
      nicla::leds.setColor(green); // Turn on the LED to indicate the connection
      while (central.connected()) {
          long currentMillis = millis();
          BHY2.update();
          // After UPDATE_FREQUENCY ms have passed, check temperature & humidity
          if (currentMillis - previousMillis >= UPDATE_FREQUENCY) {
              previousMillis = currentMillis;
              updateReadings();
          }
      }
      nicla::leds.setColor(red); // When the central disconnects, turn the LED red
      Serial.print("Disconnected from central MAC: ");
      Serial.println(central.address());
    }
}
