const int sensorPin = A0; // Pin analógico al que está conectado el sensor KY038
void setup() {
  Serial.begin(9600); // Inicia la comunicación serial a 9600 baudios
}

void loop() {
  int sensorValue = analogRead(sensorPin); // Lee el valor analógico del sensor
  Serial.println(sensorValue); // Envía el valor a la Raspberry Pi
  delay(10);
}
