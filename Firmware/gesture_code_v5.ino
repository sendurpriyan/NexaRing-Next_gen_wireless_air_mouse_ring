#include <Arduino.h>
#include <ArduinoBLE.h>
#include <Wire.h>
#include <LSM6DS3.h>  // Seeed Studio LSM6DS3 library
#include <gesture_rec_nrf52_inferencing.h> // Edge Impulse library

#define EI_CLASSIFIER_SENSOR EI_CLASSIFIER_SENSOR_ACCELEROMETER
#define CONVERT_G_TO_MS2    9.80665f
#define MAX_ACCEPTED_RANGE  2.0f
#define RUN_INFERENCE_INTERVAL_MS  200

// BLE Service and Characteristic UUIDs
BLEService imuService("19B10000-E8F2-537E-4F6C-D104768A1214");
BLECharacteristic gestureCharacteristic("19B10002-E8F2-537E-4F6C-D104768A1214", BLERead | BLENotify, 20);

static float buffer[EI_CLASSIFIER_DSP_INPUT_FRAME_SIZE * 2] = { 0 }; // Now holds both accelerometer and gyroscope data
static float inference_buffer[EI_CLASSIFIER_DSP_INPUT_FRAME_SIZE * 2]; // Adjusted size to match the new buffer
static rtos::Thread inference_thread(osPriorityLow);
LSM6DS3 myIMU(I2C_MODE, 0x6A);  // Create IMU object

void run_inference_background();
float ei_get_sign(float number);

void setup() {
    Serial.begin(115200);
    while (!Serial); // Wait for USB connection
    Serial.println("Gesture Recognition with BLE");

    // Initialize IMU
    if (myIMU.begin() != 0) {
        Serial.println("Failed to initialize IMU!");
        while (1);
    }
    Serial.println("IMU initialized");

    // Initialize BLE
    if (!BLE.begin()) {
        Serial.println("Starting BLE failed!");
        while (1);
    }
    BLE.setLocalName("XIAO nRF52840 IMU");
    BLE.setAdvertisedService(imuService);
    imuService.addCharacteristic(gestureCharacteristic);
    BLE.addService(imuService);
    
    // Start advertising to allow BLE central devices to connect
    BLE.advertise();
    Serial.println("Waiting for BLE connections...");

    // Start inference thread
    inference_thread.start(mbed::callback(&run_inference_background));
}

void loop() {
    // Check if a BLE central device is connected
    BLEDevice central = BLE.central();
    
    if (central) {
        Serial.print("Connected to central: ");
        Serial.println(central.address());

        while (central.connected()) {
            // Read sensor data and update gesture characteristic
            uint64_t next_tick = micros() + (EI_CLASSIFIER_INTERVAL_MS * 1000);
            // Shift buffer and read accelerometer and gyroscope data
            numpy::roll(buffer, EI_CLASSIFIER_DSP_INPUT_FRAME_SIZE * 2, -6); // Adjust for 6 new elements (3 accelerometer + 3 gyroscope)

            buffer[EI_CLASSIFIER_DSP_INPUT_FRAME_SIZE * 2 - 6] = myIMU.readFloatAccelX();
            buffer[EI_CLASSIFIER_DSP_INPUT_FRAME_SIZE * 2 - 5] = myIMU.readFloatAccelY();
            buffer[EI_CLASSIFIER_DSP_INPUT_FRAME_SIZE * 2 - 4] = myIMU.readFloatAccelZ();
            buffer[EI_CLASSIFIER_DSP_INPUT_FRAME_SIZE * 2 - 3] = myIMU.readFloatGyroX();
            buffer[EI_CLASSIFIER_DSP_INPUT_FRAME_SIZE * 2 - 2] = myIMU.readFloatGyroY();
            buffer[EI_CLASSIFIER_DSP_INPUT_FRAME_SIZE * 2 - 1] = myIMU.readFloatGyroZ();

            // Normalize the data
            for (int i = 0; i < 6; i++) {
                if (fabs(buffer[EI_CLASSIFIER_DSP_INPUT_FRAME_SIZE * 2 - 6 + i]) > 2.0f) {
                    buffer[EI_CLASSIFIER_DSP_INPUT_FRAME_SIZE * 2 - 6 + i] = ei_get_sign(buffer[EI_CLASSIFIER_DSP_INPUT_FRAME_SIZE * 2 - 6 + i]) * 2.0f;
                }
            }

            buffer[EI_CLASSIFIER_DSP_INPUT_FRAME_SIZE * 2 - 6] *= CONVERT_G_TO_MS2;
            buffer[EI_CLASSIFIER_DSP_INPUT_FRAME_SIZE * 2 - 5] *= CONVERT_G_TO_MS2;
            buffer[EI_CLASSIFIER_DSP_INPUT_FRAME_SIZE * 2 - 4] *= CONVERT_G_TO_MS2;

            // Wait for the next inference interval
            uint64_t time_to_wait = next_tick - micros();
            delay((int)(time_to_wait / 1000));
            delayMicroseconds(time_to_wait % 1000);
        }

        Serial.print("Disconnected from central: ");
        Serial.println(central.address());

        // Stop advertising while disconnected
        BLE.advertise();
        Serial.println("Waiting for new BLE connections...");
    }
}

float ei_get_sign(float number) {
    return (number >= 0.0) ? 1.0 : -1.0;
}

void run_inference_background() {
    delay((EI_CLASSIFIER_INTERVAL_MS * EI_CLASSIFIER_RAW_SAMPLE_COUNT) + 100);
    
    // Smooth output over multiple readings
    ei_classifier_smooth_t smooth;
    ei_classifier_smooth_init(&smooth, 10, 7, 0.8, 0.3);

    while (1) {
        memcpy(inference_buffer, buffer, EI_CLASSIFIER_DSP_INPUT_FRAME_SIZE * 2 * sizeof(float)); // Copy both accelerometer and gyroscope data
        signal_t signal;
        int err = numpy::signal_from_buffer(inference_buffer, EI_CLASSIFIER_DSP_INPUT_FRAME_SIZE * 2, &signal);
        if (err != 0) {
            Serial.println("Failed to create signal from buffer");
            return;
        }

        ei_impulse_result_t result = { 0 };
        err = run_classifier(&signal, &result, false);
        if (err != EI_IMPULSE_OK) {
            Serial.println("Failed to run classifier");
            return;
        }

        // Get the predicted gesture
        const char* prediction = ei_classifier_smooth_update(&smooth, &result);
        gestureCharacteristic.writeValue(prediction); // Send gesture over BLE

        Serial.println(prediction); // Print gesture to Serial
        delay(100); // Inference delay
    }

    ei_classifier_smooth_free(&smooth);
}
