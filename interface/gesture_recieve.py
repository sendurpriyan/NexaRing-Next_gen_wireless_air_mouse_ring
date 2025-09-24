import asyncio
import pyautogui
from bleak import BleakClient

# The BLE characteristic UUID for gesture
GESTURE_CHAR_UUID = "19B10002-E8F2-537E-4F6C-D104768A1214"

# Mapping of gestures to actions (keyboard presses)
gesture_actions = {
    "left": lambda: pyautogui.press('left'),  # Left gesture -> Left arrow key
    "right": lambda: pyautogui.press('right'),  # Right gesture -> Right arrow key
    "up": lambda: pyautogui.hotkey('shift', 'f5'),  # Up gesture -> Shift + F5
    "down": lambda: pyautogui.press('esc'),  # Down gesture -> Escape key
    "right_curl": lambda: pyautogui.press('f8'),  # Right curl -> F8
    "left_curl": lambda: pyautogui.press('f9'),  # Left curl -> F9
}

async def read_gesture(address):
    # Create a BleakClient instance to connect to the device
    async with BleakClient(address) as client:
        # Ensure we're connected to the device
        connected = await client.is_connected()
        if connected:
            print(f"Connected to {address}")

            # Read the gesture data from the BLE characteristic
            while True:
                gesture_data = await client.read_gatt_char(GESTURE_CHAR_UUID)

                # Decode the received gesture data (assuming it's UTF-8 text)
                gesture = gesture_data.decode('utf-8').strip()
                print(f"Predicted Gesture: {gesture}")

                # Check if the gesture has a corresponding action
                if gesture in gesture_actions:
                    gesture_actions[gesture]()  # Execute the corresponding action (key press)

                # Sleep for a short interval (to avoid flooding the console)
                await asyncio.sleep(1)
        else:
            print(f"Failed to connect to {address}")

# Replace with the BLE device address you're connecting to
device_address = "F4:7E:92:36:E0:77"  # Example: "00:1A:7D:DA:71:13"

# Run the read_gesture function with asyncio
loop = asyncio.get_event_loop()
loop.run_until_complete(read_gesture(device_address))
