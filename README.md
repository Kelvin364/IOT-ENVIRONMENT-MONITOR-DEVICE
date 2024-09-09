# IoT Device for Environmental Monitoring

This project is an IoT-based environmental condition monitoring system for detecting harmful gases and temperature in both remote and urban areas in real-time. The system utilizes sensors to measure carbon monoxide (CO), oxygen (O2), methane (CH4), and temperature levels, and transmits data using LoRa technology.

## Features
- **Real-Time Monitoring**: Measures environmental conditions including CO, O2, methane, and temperature in real-time.
- **LoRa Communication**: Utilizes LoRa for long-range wireless communication, making it ideal for remote monitoring.
- **Compact Design**: The device is designed to be portable and energy-efficient.
- **Display Data Locally**: Real-time sensor data is displayed on a small OLED screen for instant feedback.
- **Configurable Frequency and Bandwidth**: Supports adjustable frequency and bandwidth to comply with regional regulations.

## Hardware Components
- **Heltec LoRa Module**: A compact development board with LoRa communication and a built-in OLED display.
- **DHT11 Sensor**: Measures temperature and humidity.
- **CO, O2, Methane Sensors**: Sensors used to detect respective gas concentrations.
- **Push Button**: Turns the device on and off.

## Software Dependencies
- **Heltec LoRa Library**: For handling the communication between the device and LoRa protocol.
- **DHT Library**: For reading temperature and humidity from the DHT11 sensor.
- **Wire Library**: For I2C communication between the sensors and the microcontroller.

## How It Works
1. **Sensor Data Collection**: The device collects data from CO, O2, methane, and temperature sensors.
2. **Data Transmission**: Data is transmitted using LoRa technology to a base station or gateway for real-time monitoring.
3. **Local Display**: The OLED screen displays current sensor readings.
4. **Button Control**: A button is used to start or stop the device.

## Configuration
The following parameters can be configured:
- **Frequency**: The device operates on the 866.3 MHz frequency for Europe or 905.2 MHz for the US.
- **LoRa Bandwidth**: Set to 250 kHz by default.
- **Spreading Factor**: Set to 9 for a good balance between range and robustness.

## Code Structure

### Main Parameters
- `FREQUENCY`: Defines the operating frequency (adjustable for Europe/US).
- `BANDWIDTH`: Defines the LoRa bandwidth (default: 250 kHz).
- `SPREADING_FACTOR`: Controls the spreading factor for LoRa transmission (default: 9).

### Sensor Pins
- `CO_SENSOR_PIN`: Pin for CO sensor.
- `O2_SENSOR_PIN`: Pin for O2 sensor.
- `METHANE_SENSOR_PIN`: Pin for Methane sensor.
- `DHTPIN`: Pin for the DHT11 temperature sensor.

### Functionality
- `readSensorData()`: Reads the sensor values and converts raw readings into meaningful data.
- `displaySensorData()`: Displays sensor data on the OLED screen.
- `rx()`: Handles receiving data over LoRa.

## Installation & Setup

1. **Clone the repository**: 
   ```bash
   git clone https://github.com/your-repo/environmental-monitoring-device.git
