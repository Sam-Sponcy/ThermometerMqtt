# ThermometerMqtt
Creation of a connected thermometer using MQTT over WiFi with an Arduino Uno WiFi

Connected Thermometer with MQTT using Arduino Uno WiFi
This project implements a connected thermometer using MQTT communication over WiFi with an Arduino Uno WiFi board. The thermometer measures temperature and humidity using a DHT11 sensor and visualizes the data using NeoPixel LED strips, controlled remotely via MQTT messages.

Features
Temperature and Humidity Sensing: Utilizes DHT11 sensor to measure ambient temperature and humidity.
MQTT Communication: Connects to an MQTT broker over WiFi to publish sensor data and subscribe to control messages.
Remote LED Control: NeoPixel LED strips indicate temperature and humidity levels visually.
Task Scheduling: Utilizes TaskScheduler library to manage periodic tasks for sensor reading and LED control.
Components Used
Arduino Uno WiFi: Controls the overall logic and manages WiFi connectivity.
DHT11 Sensor: Measures temperature and humidity.
NeoPixel LED Strips: Visualizes temperature and humidity levels.
MQTT Broker: Provides communication between the Arduino and remote clients.
Libraries Required
Ensure the following libraries are installed in your Arduino IDE:

WiFiNINA
PubSubClient
DHT (for DHT11 sensor)
Adafruit_NeoPixel
TaskScheduler
Install these libraries via the Arduino Library Manager (Sketch -> Include Library -> Manage Libraries...) if they are not already installed.

Setup Instructions
Hardware Setup:

Connect the DHT11 sensor to the Arduino Uno WiFi board.
Connect NeoPixel LED strips to the designated pins.
Software Setup:

Install Arduino IDE on your computer if not already installed.
Open the Arduino sketch (*.ino) in the Arduino IDE.
Install required libraries mentioned above.
Configure WiFi and MQTT:

Modify ssid, pass, mqtt_server, mqtt_port, mqtt_user, and mqtt_password variables in the sketch to match your WiFi network and MQTT broker credentials.
Upload and Run:

Upload the sketch to your Arduino Uno WiFi board.
Open the Serial Monitor (Tools -> Serial Monitor) to view debug messages and status updates.
Usage:

The thermometer will start publishing temperature and humidity data to MQTT topics (ODC/sensor/temperature and ODC/sensor/humidity).
Subscribe to MQTT topics (ODC/led, ODC/lum, ODC/ON, ODC/OFF) to control the NeoPixel LED strips remotely.
