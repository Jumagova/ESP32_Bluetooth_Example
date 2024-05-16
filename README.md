# ESP32 Bluetooth Low Energy (BLE) Communication

This project demonstrates how to establish Bluetooth Low Energy (BLE) communication between an ESP32 microcontroller and a client device (e.g., smartphone). The ESP32 microcontroller transmits sensor data to the client device and receives control commands from the client.

## Setup

### Hardware Requirements

- ESP32 Development Board
- Sensor (e.g., analog sensor connected to pin 32)
- LED (for demonstration purposes)

### Software Requirements

- PlatformIO IDE

### Installation

1. Install the PlatformIO IDE by following the instructions on the [official website](https://platformio.org/install/ide?install=vscode).
2. Create a new project in PlatformIO IDE.
3. Copy the provided code into the `src` directory of your PlatformIO project.
4. Open the `platformio.ini` file and add the necessary dependencies for ESP32 BLE communication.
5. Build and upload the project to your ESP32 development board.

### Wiring

Connect the sensor to pin 32 on the ESP32 development board. Optionally, connect an LED to pin 2 for visual feedback.

## Usage

1. Open the serial monitor in the PlatformIO IDE to view debug messages (baud rate: 115200).
2. Scan for Bluetooth devices on your client device (e.g., smartphone).
3. Connect to the Bluetooth device named "esp32BTTest".
4. You should now be able to receive sensor data from the ESP32 and send control commands to it.

## Functionality

- The ESP32 periodically reads sensor data and transmits it to the client device via BLE.
- The ESP32 can receive control commands (e.g., to turn on/off an LED) from the client device and act accordingly.

## License

This project is licensed under the MIT License - see the [LICENSE](LICENSE) file for details.

## Acknowledgments

- This project is based on examples provided by the ESP32 Arduino Core and BLE libraries.
