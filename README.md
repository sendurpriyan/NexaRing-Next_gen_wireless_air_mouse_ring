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

### Noise filters
- Low-Pass Filter
- Average Filter Algorithm
- [Kalman Filter Algorithm](https://www.sciencedirect.com/topics/social-sciences/kalman-filter)
- Exponential Moving Average (EMA): By The Exponential Moving Average (EMA) algorithm is used to smooth the accelerometer data and reduce noise. This is crucial for achieving stable cursor movement without abrupt jumps caused by sensor inaccuracies. The algorithm computes the weighted average of current and previous acceleration values, where the formula is:


### Algorithms used for Air-mouse mode operartion
- Threshold-Based State Handling: hresholds are used to determine when the cursor should start or stop moving. The start threshold specifies the minimum acceleration required to initiate movement, while the stop threshold defines the point below which movement ceases.This approach prevents the cursor from responding to minor or unintentional sensor fluctuations.
-  Bounding and Clamping: To ensure the cursor remains within the visible screen area, the program applies a bounding and clamping algorithm. The new cursor position is calculated by adding the movement to the current position, but it is constrained between 0 and the screen’s width or height. The formula for bounding is:

<h3 align="center">new_x=max(0,min(current_x+move_x,screen_width−1))</h3>

<h3 align="center">new_y=max(0,min(current_y−move_y,screen_height−1))</h3>

This ensures that the cursor does not move off-screen, maintaining usability and preventing errors. The bounding logic is implemented in the control mouse function. 
- BLE Notification Parsing: Incoming BLE data from the NRF52840(sense) device is parsed to extract IMU and pin state information. The program expects a specific data format (six 32-bit floating-point values for accelerometer and gyroscope data, followed by three 32-bit integers for GPIO pin states). Using the struct.unpack function, it decodes the binary data into meaningful variables. If the data length does not match the expected format, an error is handled gracefully. This parsing is handled in the parse_imu_data function and called within the notification_handler.



### Algorithms used for gesture mode operartion

This proposed study used Edge Impulse for the creation, preprocessing, training, and deployment of a gesture recognition model able to categorize different hand gestures using data acquired by an IMU sensor.

1. Data Collection and Preprocessing
   - In this project, accelerator and gyroscope signals will be captured through data collection with an IMU sensor, thus allowing the effective tracking of motion corresponding to different gestures like swiping and rotation. IMU streams data in real time from the target device to the Data Acquisition tool of Edge Impulse at a sampling rate of 50 Hz, which provides enough resolution to capture peculiarities of every gesture. Data is labeled inside the Edge Impulse interface after collection. The labeling has to be done with labels indicative of different gestures: for example, "Swipe Left", "Swipe Right", "Swipe Up", "Swipe Down", "Right Rotation", "Left Rotation" and “Idle”. In this way, one will have a well-structured dataset ready for model training.
   - In this scenario of data preprocessing, instead of using a low-pass filter, the Flatten block was used for the simplification and normalization of the IMU signals. In the window statistical analysis, the Flatten block extracts the average, minimum, maximum, RMS, standard deviation, skewness, and kurtosis features in the data. These statistical measures characterize each gesture suitably, hence compacting the data for machine learning.Besides, moving average over the last N windows is applied in order to smooth the data and enhance consistency. Without filtering, this methodology removes noise of high frequency while keeping the essential gesture patterns prominent and suppressing unnecessary fluctuations in the data. Such flattened and summarized data presents a uniform low-dimensional feature set that best represents the unique signal pattern for each gesture and optimizes it for robust gesture recognition.
     <br/>
     ![feature explorer](https://github.com/user-attachments/assets/ad729cdc-7a01-4536-b44b-94c715e30492)

2. Feature Extraction
   - Critical feature extraction was done by using Edge Impulse's flatten data processing block, which enhances model performance in distinguishing between gestures. It captures raw time-domain features of essential characteristics the model can leverage due to time-based gesture patterns. Processed data is then fed into a Keras classifier for higher generalization across variance in execution of a gesture, thus enhancing the accuracy and robustness of the system.
3. Model Training and Validation
   - A Convolutional Neural Network (CNN) was chosen for its capacity to handle temporal patterns in sequential gesture data. The model was trained on Edge Impulse’s platform using a subset of the data (80% for training, 20% for validation). Model training parameters included a batch size of 32 and 100 epochs, chosen based on preliminary experimentation for optimal accuracy without overfitting. Performance metrics such as accuracy, precision, and recall were calculated to evaluate the model's ability to classify gestures accurately. Additionally, a confusion matrix provided insights into classification accuracy across gesture classes, enabling the identification of any misclassifications.
4. Model Optimization
   - Model quantization, in particular, was done considering the deployment constraints using the native 8-bit integer quantization available in Edge Impulse. It reduces model size and its memory consumption, making the model suitable for edge deployment. Further optimization included fine-tuning the model by adjusting the size of the convolution layer and the dropout rate. This strikes a good balance between model accuracy and inference speed on the target hardware.
5. Deployment on Edge Device
   - Quantization in the model is due to deployment constraints and is done using native 8-bit integer quantization available in Edge Impulse. This step reduces the model size with its memory requirements, hence making the model apt for edge deployment. The further optimization in the model was done by tweaking the size of the convolutional layer along with the dropout rate, thus trading between model accuracy and inference speed on the target hardware. Then the final model was exported in TensorFlow Lite format, tailored for edge computing on resource-constrained devices. The model was deployed on Seeed Studio Xiao nRF52840 (Sense) having an in-build IMU sensor, namely LSM6DS3TR-C, through the deployment pipeline of Edge Impulse. Then, gestures were performed in real time as a post-deployment test to check responsiveness and accuracy in a practical scenario.

### Tools and Technologies used for gesture mode
- Edge Impulse Platform: Key features used include Data Acquisition, Feature Extraction (Spectral and Time Series Analysis blocks), Model Training, and Deployment.
- Hardware: Seeed Studio Xiao nRF52840 (sense), were used for both data collection and deployment.
- Software: TensorFlow Lite and Edge Impulse’s embedded inference libraries enabled real-time, on-device gesture classification.








