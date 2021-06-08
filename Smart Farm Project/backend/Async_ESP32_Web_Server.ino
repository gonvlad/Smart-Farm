#include "WiFi.h"
#include "ESPAsyncWebServer.h"
#include "SPIFFS.h"
#include "Adafruit_Sensor.h"
#include "Adafruit_BME280.h"

#define PORT 80
#define RESOLUTION 10
#define MOISTURE_SENSOR_PIN 34
#define LIGHTING_SWITCH_PIN 27
#define WATERING_SWITCH_PIN 32
#define VENTILATION_SWITCH_PIN 33

const char* ssid = "iPhone (Владислав)";
const char* password = "thesimplestpassword";

Adafruit_BME280 bme;
AsyncWebServer server(PORT);

float temperature = 0;
float pressure = 0;
int moisture = 0;

int lightingState = 0;
int wateringState = 0;
int ventilationState = 0;
 
void setup(){
  Serial.begin(115200);

  if(!SPIFFS.begin(true)){
    Serial.println(">>> An Error has occurred while mounting SPIFFS");
    return;
  }
  
  bool status;
  status = bme.begin(0x76);  
  if (!status) {
    Serial.println(">>> An Error has occurred: could not find a valid BME280 sensor, check wiring!");
    while (1);
  }

  analogReadResolution(RESOLUTION);

  pinMode(LIGHTING_SWITCH_PIN, OUTPUT);
  pinMode(WATERING_SWITCH_PIN, OUTPUT);
  pinMode(VENTILATION_SWITCH_PIN, OUTPUT);

  WiFi.begin(ssid, password);
  while (WiFi.status() != WL_CONNECTED) {
    delay(1000);
    Serial.println(">>> Connecting to WiFi...");
  }
  Serial.println(WiFi.localIP());

  server.on("/", HTTP_GET, [](AsyncWebServerRequest *request){
    request->send(SPIFFS, "/index.html", String(), false, processor);
  });
  server.on("/style.css", HTTP_GET, [](AsyncWebServerRequest *request){
    request->send(SPIFFS, "/style.css", "text/css");
  });
  server.on("/favicon", HTTP_GET, [](AsyncWebServerRequest *request){
    request->send(SPIFFS, "/favicon.ico", "image/ico");
  });
  server.on("/logo", HTTP_GET, [](AsyncWebServerRequest *request){
    request->send(SPIFFS, "/logo.png", "image/png");
  });
  server.on("/temperature", HTTP_GET, [](AsyncWebServerRequest *request){
    request->send(SPIFFS, "/temperature.png", "image/png");
  });
  server.on("/pressure", HTTP_GET, [](AsyncWebServerRequest *request){
    request->send(SPIFFS, "/pressure.png", "image/png");
  });
  server.on("/soil", HTTP_GET, [](AsyncWebServerRequest *request){
    request->send(SPIFFS, "/soil.png", "image/png");
  });
  server.on("/light", HTTP_GET, [](AsyncWebServerRequest *request){
    request->send(SPIFFS, "/light.png", "image/png");
  });
  server.on("/watering", HTTP_GET, [](AsyncWebServerRequest *request){
    request->send(SPIFFS, "/watering.png", "image/png");
  });
  server.on("/fan", HTTP_GET, [](AsyncWebServerRequest *request){
    request->send(SPIFFS, "/fan.png", "image/png");
  });
  server.on("/gettemperature", HTTP_GET, [](AsyncWebServerRequest *request){
    request->send_P(200, "text/plain", getTemperature().c_str());
  });
  server.on("/getpressure", HTTP_GET, [](AsyncWebServerRequest *request){
    request->send_P(200, "text/plain", getPressure().c_str());
  });
  server.on("/getmoisture", HTTP_GET, [](AsyncWebServerRequest *request){
    request->send_P(200, "text/plain", getMoisture().c_str());
  });
  server.on("/switchlighting", HTTP_GET, [](AsyncWebServerRequest *request){
    request->send_P(200, "text/plain", switchLightingState().c_str());
  });
  server.on("/switchwatering", HTTP_GET, [](AsyncWebServerRequest *request){
    request->send_P(200, "text/plain", switchWateringState().c_str());
  });
  server.on("/switchventilation", HTTP_GET, [](AsyncWebServerRequest *request){
    request->send_P(200, "text/plain", switchVentilationState().c_str());
  });
  server.begin();
}
 
void loop(){
  
}

String processor(const String& state) {
  if (state == "TEMPERATURE") {
    return getTemperature();
  } else if (state == "PRESSURE") {
    return getPressure();
  } else if (state == "MOISTURE") {
    return getMoisture();
  }
  return String();
}

String getTemperature() {
  float sensorValue = bme.readTemperature();
  if (sensorValue == sensorValue) {
    temperature = sensorValue;
  }
  return String(temperature);
}

String getPressure() {
  float sensorValue = bme.readPressure();
  if (sensorValue == sensorValue) {
    pressure = (sensorValue / 100.0F) / 1.333;
  }
  return String(pressure);
}

String getMoisture() {
  moisture = analogRead(MOISTURE_SENSOR_PIN);
  moisture = map(moisture, 750, 180, 0, 100); 
  return String(moisture);
}

String switchLightingState() {
  if (lightingState == 0) {
    digitalWrite(LIGHTING_SWITCH_PIN, HIGH);
    lightingState = 1;
    Serial.println(">>> Lighting state is UP");
  } else {
    digitalWrite(LIGHTING_SWITCH_PIN, LOW);
    lightingState = 0;
    Serial.println(">>> Lighting state is DOWN");
  }
  return String();
}

String switchWateringState() {
  if (wateringState == 0) {
    digitalWrite(WATERING_SWITCH_PIN, HIGH);
    wateringState = 1;
    Serial.println(">>> Watering state is UP");
  } else {
    digitalWrite(WATERING_SWITCH_PIN, LOW);
    wateringState = 0;
    Serial.println(">>> Watering state is DOWN");
  }
  return String();
}

String switchVentilationState() {
  if (ventilationState == 0) {
    digitalWrite(VENTILATION_SWITCH_PIN, HIGH);
    ventilationState = 1;
    Serial.println(">>> Ventilation state is UP");
  } else {
    digitalWrite(VENTILATION_SWITCH_PIN, LOW);
    ventilationState = 0;
    Serial.println(">>> Ventilation state is DOWN");
  }
  return String();
}
