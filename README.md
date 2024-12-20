# NexaRing: Next-Generation Wireless Air Mouse Ring

This project showcases a compact, wearable air mouse ring that combines convenience and functionality for seamless interaction with devices. Designed for next-generation applications, it uses motion sensors to track gestures, enabling precise cursor control and gesture-based inputs without a traditional mouse. The ring communicates wirelessly (e.g., via Bluetooth) with your computer or smart device, offering an ergonomic and futuristic alternative for presentations, gaming, or daily use.


## Objectives

By removing the limitations of conventional mouse systems, this novel method promises to 
improve user mobility and intuitive engagement. It is especially helpful in situations like 
business travel, academic conferences, and mobile officiating. 

- create the air mouse ring innovative and tiny as it wearable to human fingers. User could be able to use the mouse with convenience. 
- The mouse could be used in three-dimensional space volume which refers air mouse mode and it should contain basic mouse operations in very user-friendly way. 
- As an additional feature of gesture mode should be implemented to the air mouse; from these gesture mode users can perform some gesture controlling operations to the pc. 
- The device should be compatible with various types of personal computers. Such are, laptops, desktops, tablets, mobiles etc. 
- Create the mouse device which is rechargeable and could work for more time in single charge.

## Methodology

### Flow chart of complete process
![Flow chart of complete process](https://github.com/user-attachments/assets/dfc67436-18ee-4848-a1ba-0ba9abc9ebd1)

### Data Acquisition from IMU Sensor Embedded in Nrf52840 (Sense)  
The nRF52840 Sense device, equipped with an embedded IMU sensor, captures real-time movement data. The IMU sensor measures acceleration, gyroscopic data, and sometimes magnetometer data, providing comprehensive motion tracking in multiple axes. The nRF52840 Sense operates using BLE for seamless wireless communication. 


### Data Transfer through Bluetooth Low Energy (BLE) 
As soon as the filtered data has been processed, the movement data collected from the sensor is translated to an appropriate mouse click or specific control action. This translation includes the encoding of motion data such as motion in x-axis, y-axis and z-axis spaces into codes for a computer mouse to control clicks, scrolling and movement of the cursor on the screen. Calibration provides the correlation between the mouse movements and the sensor movements ensuring that the linking is accurate and provides natural interaction. 


### Data Import and Processing 
The incoming IMU sensor data is imported into the PC during the next step of processing. My raw data contains information, which leads to noise from time to time while due to the interferences from the surroundings or imperfections of the sensor. The signal is filtered to remove noise and retain significant signals before further handling of the data. A noise filtering algorithm is applied to the data stream to eliminate unnecessary fluctuations and inaccuracies. This step ensures that only clean and accurate motion data is processed, improving the reliability of the system for gesture recognition and mouse control applications. 


### Noise Filtering 
A noise filtering algorithm is applied to the data stream to eliminate unnecessary fluctuations and inaccuracies. This step ensures that only clean and accurate motion data is processed, improving the reliability of the system for gesture recognition and mouse control applications. 


### Data to Mouse Code Conversion 
Once the data from the sensor that has been filtered is completed, it is thereafter associated with the screen clicks or control movements. A translation step that is simply applicable can be restated. This translation step involves converting motion data of the form X, Y and Z coordinates to mouse control codes which include clicking, scrolling or cursor movement. This is what calibration does it provides a mapping between mouse controls and sensor data which allows achieving natural interaction.


### Special Gesture Recognition 
For actions, unique gestures are defined in addition to regular motion tracking. These gestures are designed to initiate particular computer operations, like shortcut instructions or distinctive mouse motions,and are identified by distinctive patterns in the IMU sensor data. With the addition of an additional layer of control, preset custom gestures enable improved engagement.


## Hardwares
- [Seeed Studio Xiao nRF52840 BLE sense](https://wiki.seeedstudio.com/XIAO_BLE/) : With its built-in inbuilt antenna for optimal performance in wireless connection, the device's Bluetooth 5.0 wireless technology ensures strong connectivity. It has a high processing efficiency for demanding activities because it is powered by the Nordic nRF52840 SoC and has an ARM® Cortex®-M4 32-bit processor with a floating-point unit (FPU) operating at 64 MHz. The device is also equipped with an onboard PDM microphone for sound input and a 6-axis LSM6DS3TR-C IMU for motion sensing, providing a wide range of sensing and interactive capabilities. 
- [401220 3.7v 65mah Li-Po Rechargeable Battery](https://www.lipobattery.us/lipo-battery-lp401220-3-7v-80mah-0-3wh-without-protection-circuit-but-with-wires-5mm/).
- Push Buttons.
 

### Final Product
![Final Product](https://github.com/user-attachments/assets/e4766d3e-fe40-4674-8c7f-42a698652ba6)


## Softwares

