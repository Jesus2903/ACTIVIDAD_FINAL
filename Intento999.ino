#include <Wire.h>
#include "MAX30105.h"
#include <WiFi.h>
#include <PubSubClient.h>

MAX30105 particleSensor;

// Credenciales de Wi-Fi
const char* ssid = "INFINITUM1CF8";
const char* password = "YSmu57YXGY";

// Configuración de ThingSpeak
const char* mqttServer = "mqtt3.thingspeak.com";
const int mqttPort = 1883;
const char* mqttUser = "EiQWECIbKTQkCg0kOCEVNj0"; // Client ID
const char* mqttPassword = "0aqgqls3qoUBg8dVj0AwYjYz";
const char* channelID = "2775087";
const char* writeAPIKey = "6WHQBWT1MNOZZ3NC";

WiFiClient espClient;
PubSubClient client(espClient);

void setup() {
  Serial.begin(115200);

  // Conectar a Wi-Fi
  Serial.print("Conectando a WiFi...");
  WiFi.begin(ssid, password);

  while (WiFi.status() != WL_CONNECTED) {
    delay(1000);
    Serial.println("Intentando conectar a WiFi...");
  }
  Serial.println("WiFi conectado. IP: " + WiFi.localIP().toString());

  // Configurar MQTT
  client.setServer(mqttServer, mqttPort);

  if (!client.connect(mqttUser, mqttUser, mqttPassword)) { // Usa Client ID, username y password
    Serial.println("Error al conectar al broker MQTT");
    while (1);
  }
  Serial.println("Conexión exitosa a ThingSpeak MQTT");
}

void loop() {
  // Simular datos del sensor
  int pulsoCardiaco = random(60, 100); // Pulso cardíaco simulado
  int oxigeno = random(90, 100);      // Oxígeno simulado

  // Crear payload y topic
  String payload = "field1=" + String(pulsoCardiaco) + "&field2=" + String(oxigeno) + "&api_key=" + String(writeAPIKey);
  String topic = "channels/" + String(channelID) + "/publish";

  // Mostrar topic y payload
  Serial.println("Topic: " + topic);
  Serial.println("Payload: " + payload);

  // Enviar datos
  if (client.publish(topic.c_str(), payload.c_str())) {
    Serial.println("Datos enviados a ThingSpeak:");
    Serial.println("Payload: " + payload);
  } else {
    Serial.println("Error al enviar datos a ThingSpeak");
  }

  // Esperar 15 segundos
  delay(15000);
}

