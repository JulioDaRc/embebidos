#include <ESP8266WiFi.h>
#include <PubSubClient.h>

// Configura tus credenciales de WiFi
const char* ssid = "julio";
const char* password = "12345678";

// Configura las credenciales de tu broker MQTT
const char* mqtt_server = "192.168.137.106";
const int mqtt_port = 1883;

WiFiClient espClient;
PubSubClient client(espClient);


bool keepRunning = true;

// Pines de los LEDs
const int greenLedPin1 = 1;
const int redLedPin1 = 3;
const int greenLedPin2 = 15;
const int redLedPin2 = 13;
const int greenLedPin3 = 12;
const int redLedPin3 = 14;
const int greenLedPin4 = 5;
const int redLedPin4 = 16;

void setup_wifi() {
    delay(10);
    // Conectando a WiFi
    WiFi.begin(ssid, password);
    while (WiFi.status() != WL_CONNECTED) {
        delay(50);
    }
}


void callback(char* topic, byte* payload, unsigned int length) {
    payload[length] = '\0';
    String message = String((char*)payload);
    if (String(topic) == "SecuenciaSem") {
        if (message == "on") {
            keepRunning = true;
            ejecutarSecuencia();
        } else if (message == "off") {
            keepRunning = false;
                    digitalWrite(greenLedPin1, LOW);
    digitalWrite(redLedPin1, LOW);
    digitalWrite(greenLedPin2, LOW);
    digitalWrite(redLedPin2, LOW);
    digitalWrite(greenLedPin3, LOW);
    digitalWrite(redLedPin3, LOW);
    digitalWrite(greenLedPin4, LOW);
    digitalWrite(redLedPin4, LOW);
        }
    }
}

void reconnect() {
    // Loop hasta que nos reconectemos
    while (!client.connected()) {
        if (client.connect("ESP8266Client")) {
            client.subscribe("SecuenciaSem");
        }
        delay(50);
    }
}

void setup() {
    Serial.begin(115200);
    setup_wifi();
    client.setServer(mqtt_server, mqtt_port);
    client.setCallback(callback);

    // Inicializa los pines de los LEDs
    pinMode(greenLedPin1, OUTPUT);
    pinMode(redLedPin1, OUTPUT);
    pinMode(greenLedPin2, OUTPUT);
    pinMode(redLedPin2, OUTPUT);
    pinMode(greenLedPin3, OUTPUT);
    pinMode(redLedPin3, OUTPUT);
    pinMode(greenLedPin4, OUTPUT);
    pinMode(redLedPin4, OUTPUT);
    publicarMensaje();
}

void loop() {
    if (!client.connected()) {
        reconnect();
    }
    client.loop();
}

void ejecutarSecuencia() {
    while(keepRunning==true){
    digitalWrite(greenLedPin1, HIGH);
    digitalWrite(redLedPin1, LOW);
    digitalWrite(greenLedPin2, LOW);
    digitalWrite(redLedPin2, HIGH);
    digitalWrite(greenLedPin3, LOW);
    digitalWrite(redLedPin3, HIGH);
    digitalWrite(greenLedPin4, LOW);
    digitalWrite(redLedPin4, HIGH);
    client.loop();
    delay(3500);
    
        digitalWrite(greenLedPin1, LOW);
    digitalWrite(redLedPin1, HIGH);
    digitalWrite(greenLedPin2, HIGH);
    digitalWrite(redLedPin2, LOW);
    digitalWrite(greenLedPin3, LOW);
    digitalWrite(redLedPin3, HIGH);
    digitalWrite(greenLedPin4, LOW);
    digitalWrite(redLedPin4, HIGH);
    client.loop();
    delay(3500);

     digitalWrite(greenLedPin1, LOW);
    digitalWrite(redLedPin1, HIGH);
    digitalWrite(greenLedPin2, LOW);
    digitalWrite(redLedPin2, HIGH);
    digitalWrite(greenLedPin3, HIGH);
    digitalWrite(redLedPin3, LOW);
    digitalWrite(greenLedPin4, LOW);
    digitalWrite(redLedPin4, HIGH);
    client.loop();
    delay(3500);

         digitalWrite(greenLedPin1, LOW);
    digitalWrite(redLedPin1, HIGH);
    digitalWrite(greenLedPin2, LOW);
    digitalWrite(redLedPin2, HIGH);
    digitalWrite(greenLedPin3, LOW);
    digitalWrite(redLedPin3, HIGH);
    digitalWrite(greenLedPin4, HIGH);
    digitalWrite(redLedPin4, LOW);
    client.loop();
    delay(3500);
}
}

void publicarMensaje() {
   if (!client.connected()) {
    reconnect();
  }
    client.publish("SecuenciaSem", "on");
}
