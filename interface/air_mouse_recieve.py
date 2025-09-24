import asyncio  # For asynchronous programming
import struct  # For handling binary data (packing/unpacking)
import pyautogui  # For controlling the mouse cursor
from bleak import BleakClient  # Importing BleakClient for BLE communication

# BLE constants
ble_address = "F4:7E:92:36:E0:77"  # Replace with your ESP32's BLE address
imu_characteristic_uuid = "19B10002-E8F2-537E-4F6C-D104768A1214"  # UUID for the IMU characteristic

pyautogui.FAILSAFE = False  # Disable PyAutoGUI's fail-safe mechanism
SCREEN_WIDTH, SCREEN_HEIGHT = pyautogui.size()  # Get screen resolution

# Sensitivity settings
MIN_SENSITIVITY = 0.00001
MAX_SENSITIVITY = 3600
X_SENSITIVITY_MULTIPLIER = 1.2
Y_SENSITIVITY_MULTIPLIER = 2
THRESHOLD = 0.01  # Minimum threshold for movement detection

# Button click state variables
left_click_state = False
right_click_state = False

# Calculate sensitivity based on predicted distance
def calculate_sensitivity(predicted_distance):
    MAX_PREDICTED_DISTANCE = 95
    normalized_distance = max(0, min(predicted_distance / MAX_PREDICTED_DISTANCE, 1))
    sensitivity = MIN_SENSITIVITY + (normalized_distance * (MAX_SENSITIVITY - MIN_SENSITIVITY))
    return int(sensitivity)

# Predict movement distance based on accelerometer data
def predict_movement_distance(accel_x, accel_y):
    accel_magnitude = (accel_x ** 2 + accel_y ** 2) ** 0.5
    return accel_magnitude

# Mouse control based on accelerometer data
def control_mouse(accel_x, accel_y, pin3):
    if pin3 and (abs(accel_x) > THRESHOLD or abs(accel_y) > THRESHOLD):
        predicted_distance = predict_movement_distance(accel_x, accel_y)

        # Calculate separate sensitivities for x and y axes
        current_sensitivity_x = calculate_sensitivity(predicted_distance) * X_SENSITIVITY_MULTIPLIER
        current_sensitivity_y = calculate_sensitivity(predicted_distance) * Y_SENSITIVITY_MULTIPLIER

        current_x, current_y = pyautogui.position()

        # Apply movement with calculated sensitivities
        new_x = current_x + int(accel_y * current_sensitivity_y)
        new_y = current_y - int(accel_x * current_sensitivity_x)

        # Bound the new position within screen dimensions
        new_x = max(0, min(new_x, SCREEN_WIDTH - 1))
        new_y = max(0, min(new_y, SCREEN_HEIGHT - 1))

        pyautogui.moveTo(new_x, new_y, _pause=False)
        print(f"Moved mouse to ({new_x}, {new_y}) with x-sensitivity {current_sensitivity_x} and y-sensitivity {current_sensitivity_y}")
    else:
        print("Pin3 inactive or movement below threshold: Cursor movement disabled.")

# Perform mouse clicks based on pin states
def perform_mouse_clicks(pin1_state, pin2_state):
    global left_click_state, right_click_state

    # Right-click (Pin1)
    if pin1_state and not right_click_state:
        pyautogui.rightClick()
        right_click_state = True
        print("Right click performed.")

    if not pin1_state:
        right_click_state = False

    # Left-click (Pin2)
    if pin2_state and not left_click_state:
        pyautogui.click()
        left_click_state = True
        print("Left click performed.")

    if not pin2_state:
        left_click_state = False

# Parse incoming BLE data
def parse_imu_data(data):
    try:
        imu_values = struct.unpack('6f3i', data)  # Expecting 6 floats and 3 integers
        return imu_values
    except struct.error as e:
        print(f"Error unpacking data: {e}. Unexpected data length: {len(data)} bytes.")
        return None

# Notification handler
def notification_handler(sender, data):
    imu_values = parse_imu_data(data)
    if imu_values is None:
        return

    accel_x, accel_y, accel_z, gyro_x, gyro_y, gyro_z, pin1, pin2, pin3 = imu_values

    # Print received data for debugging
    print(f"Received IMU Data - Accel: ({accel_x:.2f}, {accel_y:.2f}), "
          f"Gyro: ({gyro_x:.2f}, {gyro_y:.2f}), Pins: ({pin1}, {pin2}, {pin3})")

    # Control cursor movement
    control_mouse(accel_x, accel_y, pin3)

    # Perform mouse clicks
    perform_mouse_clicks(pin1, pin2)

# Main BLE connection and notification loop
async def main():
    async with BleakClient(ble_address) as client:
        print(f"Connected to {ble_address}")
        await client.start_notify(imu_characteristic_uuid, notification_handler)
        await asyncio.sleep(10000)  # Keep the connection alive for a long duration


asyncio.run(main())