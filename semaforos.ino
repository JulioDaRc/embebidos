#include <WiFi.h>
#include <PubSubClient.h>

// Configuración de la red WiFi
const char* ssid = "julio";
const char* password = "12345678";

// Configuración del servidor MQTT
const char* mqtt_server = "192.168.137.106";

// Configuración de los pines de los LEDs
const int greenLedPin1 = 15;
const int redLedPin1 = 2;
const int greenLedPin2 = 5;
const int redLedPin2 = 18;
const int greenLedPin3 = 19;
const int redLedPin3 = 21;
const int greenLedPin4 = 22;
const int redLedPin4 = 23;

WiFiClient espClient;
PubSubClient client(espClient);

void setup_wifi() {
  delay(10);
  // Conexión a la red WiFi
  WiFi.begin(ssid, password);

  while (WiFi.status() != WL_CONNECTED) {
    delay(500);
  }
}

void callback(char* topic, byte* message, unsigned int length) {
  String messageTemp;

  for (int i = 0; i < length; i++) {
    messageTemp += (char)message[i];
  }

  if (String(topic) == "Sen1") {
    if (messageTemp == "Emergency") {
      digitalWrite(greenLedPin1, HIGH);
      digitalWrite(redLedPin1, LOW);
      client.publish("Semaforo1", "on");
    } else if (messageTemp == " ") {
      digitalWrite(greenLedPin1, LOW);
      digitalWrite(redLedPin1, HIGH);
      client.publish("Semaforo1", "off");
    }
  }

  if (String(topic) == "Sen2") {
    if (messageTemp == "Emergency") {
      digitalWrite(greenLedPin2, HIGH);
      digitalWrite(redLedPin2, LOW);
      client.publish("Semaforo2", "on");
    } else if (messageTemp == " ") {
      digitalWrite(greenLedPin2, LOW);
      digitalWrite(redLedPin2, HIGH);
      client.publish("Semaforo2", "off");
    }
  }
if (String(topic) == "Sen3") {
    if (messageTemp == "Emergency") {
      digitalWrite(greenLedPin3, HIGH);
      digitalWrite(redLedPin3, LOW);
      client.publish("Semaforo3", "on");
    } else if (messageTemp == " ") {
      digitalWrite(greenLedPin3, LOW);
      digitalWrite(redLedPin3, HIGH);
      client.publish("Semaforo3", "off");
    }
  }
  if (String(topic) == "Sen4") {
    if (messageTemp == "Emergency") {
      digitalWrite(greenLedPin4, HIGH);
      digitalWrite(redLedPin4, LOW);
      client.publish("Semaforo4", "on");
    } else if (messageTemp == " ") {
      digitalWrite(greenLedPin4, LOW);
      digitalWrite(redLedPin4, HIGH);
      client.publish("Semaforo4", "off");
    }
  }

  
}

void reconnect() {
  while (!client.connected()) {
    if (client.connect("ESP32Client")) {
      client.subscribe("Sen1");
      client.subscribe("Sen2");
      client.subscribe("Sen3");
      client.subscribe("Sen4");
    } else {
      delay(5000);
    }
  }
}

void setup() {
  pinMode(greenLedPin1, OUTPUT);
  pinMode(redLedPin1, OUTPUT);
  pinMode(greenLedPin2, OUTPUT);
  pinMode(redLedPin2, OUTPUT);
  pinMode(greenLedPin3, OUTPUT);
  pinMode(redLedPin3, OUTPUT);
  pinMode(greenLedPin4, OUTPUT);
  pinMode(redLedPin4, OUTPUT);
  digitalWrite(redLedPin1, HIGH);
  digitalWrite(redLedPin2, HIGH);
  digitalWrite(redLedPin3, HIGH);
  digitalWrite(redLedPin4, HIGH);
  setup_wifi();
  client.setServer(mqtt_server, 1883);
  client.setCallback(callback);
}

void loop() {
  if (!client.connected()) {
    reconnect();
  }
  client.loop();
}
