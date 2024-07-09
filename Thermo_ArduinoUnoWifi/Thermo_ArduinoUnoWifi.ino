#include <SPI.h>
#include <WiFiNINA.h>
#include <PubSubClient.h>
#include <DHT.h>
#include <Adafruit_NeoPixel.h>
#include <TaskScheduler.h>

// DHT sensor setup
#define DHTPIN 4
#define DHTTYPE DHT11

// LED strip setup
#define PIN 6
#define PIN2 5
#define NUM_LEDS 30
#define BRIGHTNESS 100

// Humidity and temperature thresholds
#define HUMIDITY_MIN 20
#define HUMIDITY_MAX 60
#define TEMPERATURE_MIN 10
#define TEMPERATURE_MAX 40
#define LED_MIN 0
#define LED_MAX 30

// Initialize NeoPixel strips for humidity and temperature
Adafruit_NeoPixel strip_H = Adafruit_NeoPixel(NUM_LEDS, PIN, NEO_GRB + NEO_KHZ800);
Adafruit_NeoPixel strip_T = Adafruit_NeoPixel(NUM_LEDS, PIN2, NEO_GRB + NEO_KHZ800);

// Initialize DHT sensor
DHT dht(DHTPIN, DHTTYPE);

// WiFi and MQTT credentials
char ssid[] = "Your SSID";
char pass[] = "password";
const char* mqtt_server = "0.0.0.0";
const int mqtt_port = 1883;
const char* mqtt_user = "";
const char* mqtt_password = "";

// WiFi and MQTT client setup
WiFiClient espClient;
PubSubClient client(espClient);
int status = WL_IDLE_STATUS;

// Global variables for storing MQTT messages and sensor data
String led_color = "";
int led_lum = 0;
String led_on_off = "";
float temperature;
float humidity;

// Task scheduler setup
Scheduler runner;

// Task declarations
void readDHT();
void controlLED();
Task taskDHT(10000, TASK_FOREVER, &readDHT);
Task taskLED(1000, TASK_FOREVER, &controlLED);

void setup() {
  Serial.begin(9600);
  dht.begin();
  strip_H.begin();
  strip_H.setBrightness(BRIGHTNESS);
  strip_H.show();
  strip_T.begin();
  strip_T.setBrightness(BRIGHTNESS);
  strip_T.show();

  // Connect to WiFi network
  while (status != WL_CONNECTED) {
    Serial.print("Attempting to connect to WiFi...");
    status = WiFi.begin(ssid, pass);
    delay(5000);
  }

  Serial.println("Connected to WiFi!");

  // Connect to MQTT server
  client.setServer(mqtt_server, mqtt_port);
  client.setCallback(callback);

  while (!client.connected()) {
    Serial.print("Attempting to connect to MQTT server...");
    if (client.connect("ESP32Client", mqtt_user, mqtt_password)) {
      Serial.println("Connected to MQTT server!");
      client.subscribe("ODC/led");
      client.subscribe("ODC/lum");
      client.subscribe("ODC/ON");
      client.subscribe("ODC/OFF");
    } else {
      Serial.print("Connection failed, rc=");
      Serial.print(client.state());
      Serial.println(" trying again in 5 seconds");
      delay(5000);
    }
  }

  // Add tasks to the scheduler
  runner.addTask(taskDHT);
  runner.addTask(taskLED);
  taskDHT.enable();
  taskLED.enable();
}

void loop() {
  if (!client.connected()) {
    reconnect();
  }
  client.loop();
  runner.execute();
}

// MQTT callback function
void callback(char* topic, byte* payload, unsigned int length) {
  Serial.print("Message received on topic [");
  if (strcmp(topic, "ODC/led") == 0) {
    Serial.print(topic);
    Serial.print("]: ");
    led_color = "";
    for (unsigned int i = 0; i < length; i++) {
      led_color += (char)payload[i];
    }
    Serial.println(led_color);
  } else if (strcmp(topic, "ODC/OFF") == 0 || strcmp(topic, "ODC/ON") == 0) {
    Serial.print("test");
    Serial.print("]: ");
    led_on_off = "";
    for (unsigned int i = 0; i < length; i++) {
      led_on_off += (char)payload[i];
    }
    if (led_on_off == "OFF") {
      strip_H.setBrightness(0);
      strip_H.show();
      strip_T.setBrightness(0);
      strip_T.show();
    } else if (led_on_off == "ON") {
      strip_H.setBrightness(BRIGHTNESS);
      strip_H.show();
      strip_T.setBrightness(BRIGHTNESS);
      strip_T.show();
    }
    Serial.println(led_on_off);
  } else if (strcmp(topic, "ODC/lum") == 0) {
    char lumStr[length + 1];
    for (unsigned int i = 0; i < length; i++) {
      lumStr[i] = (char)payload[i];
    }
    lumStr[length] = '\0';
    led_lum = atoi(lumStr);
    float led_lum2 = led_lum / 2.55;
    Serial.println(led_lum2);
    strip_H.setBrightness(led_lum2);
    strip_H.show();
    strip_T.setBrightness(led_lum2);
    strip_T.show();
  } else {
    Serial.println("error");
  }
}

// Reconnect to the MQTT server
void reconnect() {
  while (!client.connected()) {
    Serial.print("Attempting to reconnect to MQTT server...");
    if (client.connect("ESP32Client", mqtt_user, mqtt_password)) {
      Serial.println("Reconnected to MQTT server!");
      client.subscribe("ODC/led");
    } else {
      Serial.print("Reconnection failed, rc=");
      Serial.print(client.state());
      Serial.println(" trying again in 5 seconds");
      delay(5000);
    }
  }
}

// Task to read DHT sensor data
void readDHT() {
  temperature = dht.readTemperature();
  humidity = dht.readHumidity();

  if (isnan(temperature) || isnan(humidity)) {
    Serial.println("Error reading sensor!");
    return;
  }

  Serial.print("Temperature: ");
  Serial.print(temperature);
  Serial.print(" °C\t");
  Serial.print("Humidity: ");
  Serial.println(humidity);

  char tempStr[8];
  dtostrf(temperature, 6, 2, tempStr);

  char humStr[8];
  dtostrf(humidity, 6, 2, humStr);

  client.publish("ODC/sensor/temperature", tempStr);
  client.publish("ODC/sensor/humidity", humStr);
}

// Task to control the LED strips
void controlLED() {
  int nb_led_H = map(humidity, HUMIDITY_MIN, HUMIDITY_MAX, LED_MIN, LED_MAX);
  nb_led_H = constrain(nb_led_H, LED_MIN, LED_MAX);
  Serial.print("Number of Humidity LEDs: ");
  Serial.println(nb_led_H);

  int nb_led_T = map(temperature, TEMPERATURE_MIN, TEMPERATURE_MAX, LED_MIN, LED_MAX);
  nb_led_T = constrain(nb_led_T, LED_MIN, LED_MAX);
  Serial.print("Number of Temperature LEDs: ");
  Serial.println(nb_led_T);

  uint32_t color_T = getColorFromValue_T(nb_led_T);
  setStripColor_T(color_T, nb_led_T);
  uint32_t color_H = getColorFromValue_H(nb_led_H);
  setStripColor_H(color_H, nb_led_H);
}

// Get color based on temperature value
uint32_t getColorFromValue_T(int nb_led_T) {
  byte r, g, b;

  if (nb_led_T <= 15) {
    r = 0;
    g = map(nb_led_T, 0, 15, 0, 255);
    b = map(nb_led_T, 0, 15, 255, 0);
  } else {
    r = map(nb_led_T, 16, 30, 0, 255);
    g = map(nb_led_T, 16, 30, 255, 0);
    b = 0;
  }

  return strip_T.Color(r, g, b);
}

// Get color based on humidity value
uint32_t getColorFromValue_H(int value) {
  byte r, g, b;

  if (value <= 15) {
    r = map(value, 0, 15, 255, 0);
    g = map(value, 0, 15, 0, 255);
    b = 0;
  } else {
    r = 0;
    g = map(value, 16, 30, 255, 0);
    b = map(value, 16, 30, 0, 255);
  }

  return strip_H.Color(r, g, b);
}

// Set color for temperature LED strip
void setStripColor_T(uint32_t color_T, int nb_led_T) {
  for (int i = 0; i < NUM_LEDS; i++) {
    if (i < nb_led_T) {
      strip_T.setPixelColor(i, color_T);
    } else {
      strip_T.setPixelColor(i, strip_T.Color(0, 0, 0));
    }
  }
  strip_T.show();
}

// Set color for humidity LED strip
void setStripColor_H(uint32_t color_H, int nb_led_H) {
  for (int i = 0; i < NUM_LEDS; i++) {
    if (i < nb_led_H) {
      strip_H.setPixelColor(i, color_H);
    } else {
      strip_H.setPixelColor(i, strip_H.Color(0, 0, 0));
    }
  }
  strip_H.show();
}
