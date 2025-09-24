#include <ArduinoBLE.h>
#include <Wire.h>
#include <LSM6DS3.h>  // Example IMU sensor library for LSM6DS3

BLEService imuService("19B10000-E8F2-537E-4F6C-D104768A1214");  // Bluetooth® Low Energy IMU Service

// Bluetooth® Low Energy IMU Data Characteristic (can hold binary data)
BLECharacteristic imuCharacteristic("19B10002-E8F2-537E-4F6C-D104768A1214", BLERead | BLENotify, sizeof(float) * 6 + sizeof(int) * 3);

LSM6DS3 imu;  // Create IMU object

// Digital input pins
const int Pin1 = 0;
const int Pin2 = 1;
const int Pin3 = 2;

void setup() {
  Serial.begin(9600);
  Wire.begin();  // Initialize I2C for IMU communication

  // Initialize IMU
  if (imu.begin() != 0) {
    Serial.println("IMU initialization failed!");
    while (1);
  }

  // Set input pins with pull-up resistors
  pinMode(Pin1, INPUT_PULLDOWN);
  pinMode(Pin2, INPUT_PULLDOWN);
  pinMode(Pin3, INPUT_PULLDOWN);

  // Begin BLE initialization
  if (!BLE.begin()) {
    Serial.println("Starting BLE failed!");
    while (1);
  }

  // Set BLE device name and service
  BLE.setLocalName("XIAO nRF52840 IMU");
  BLE.setAdvertisedService(imuService);

  // Add IMU characteristic to the service
  imuService.addCharacteristic(imuCharacteristic);

  // Add the service to BLE
  BLE.addService(imuService);

  // Start advertising the service
  BLE.advertise();

  Serial.println("Waiting for BLE connections...");
}

void loop() {
  BLEDevice central = BLE.central();

  if (central) {
    Serial.print("Connected to central: ");
    Serial.println(central.address());

    while (central.connected()) {
      // Read IMU data
      float accelX = imu.readFloatAccelX();
      float accelY = imu.readFloatAccelY();
      float accelZ = imu.readFloatAccelZ();
      float gyroX = imu.readFloatGyroX();
      float gyroY = imu.readFloatGyroY();
      float gyroZ = imu.readFloatGyroZ();

      // Read the digital inputs
      int btn1 = digitalRead(Pin1);
      int btn2 = digitalRead(Pin2);
      int btn3 = digitalRead(Pin3);

      // Create an array to hold IMU data and pin states
      float imuData[9] = { accelX, accelY, accelZ, gyroX, gyroY, gyroZ, btn1, btn2, btn3 };

      // Send IMU data and pin states over BLE
      imuCharacteristic.writeValue((byte*)imuData, sizeof(imuData));

      // Print IMU data and pin states to the Serial monitor for debugging
      Serial.println("Data Sent:");
      Serial.print("AccelX: "); Serial.print(accelX);
      Serial.print(" AccelY: "); Serial.print(accelY);
      Serial.print(" AccelZ: "); Serial.print(accelZ);
      Serial.print(" GyroX: "); Serial.print(gyroX);
      Serial.print(" GyroY: "); Serial.print(gyroY);
      Serial.print(" GyroZ: "); Serial.println(gyroZ);

      // Print the states of the digital inputs
      Serial.print("Input Pin 1: "); Serial.print(btn1);
      Serial.print(" Input Pin 5: "); Serial.print(btn2);
      Serial.print(" Input Pin 9: "); Serial.println(btn3);

      delay(1);  // Delay between readings
    }

    Serial.print("Disconnected from central: ");
    Serial.println(central.address());
  }
}