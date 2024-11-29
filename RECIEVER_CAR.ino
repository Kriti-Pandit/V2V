#include <TinyGPS++.h>
#include <SoftwareSerial.h>
#include <SPI.h>
#include <nRF24L01.h>
#include <RF24.h>

// The serial connection to the GPS module
SoftwareSerial gpsSerial(3, 4); // RX, TX

TinyGPSPlus gps;

RF24 radio(9, 10); // CE, CSN
const byte address[6] = "00001";

const int trigPin = 4;
const int echoPin = 5;

long duration;
int distance;

void setup() {
  Serial.begin(9600);
  gpsSerial.begin(9600);
  radio.begin();
  radio.openWritingPipe(address);
  radio.setPALevel(RF24_PA_MAX);
  radio.stopListening();

  pinMode(trigPin, OUTPUT);
  pinMode(echoPin, INPUT);

  Serial.println(F("GPS Module test"));
}

void loop() {
  // This sketch displays information every time a new sentence is correctly encoded.
  while (gpsSerial.available() > 0) {
    if (gps.encode(gpsSerial.read())) {
      displayInfo();
    }
  }

  // If 5000ms pass without any data, something may be wrong.
  if (millis() > 5000 && gps.charsProcessed() < 10) {
    Serial.println(F("No GPS detected: check wiring."));
    while(true);
  }

  // Ultrasonic sensor code
  digitalWrite(trigPin, LOW);
  delayMicroseconds(2);
  digitalWrite(trigPin, HIGH);
  delayMicroseconds(10);
  digitalWrite(trigPin, LOW);
  duration = pulseIn(echoPin, HIGH);
  distance = duration * 0.034 / 2;

  // Print the distance to the serial monitor
  Serial.print("Distance: ");
  Serial.print(distance);
  Serial.println(" cm");

  // Check if the distance is less than 100 cm (1 meter)
  if (distance < 100) {
    // Transmit distance data and GPS location using NRF24L01 module
    radio.write(&distance, sizeof(distance));
    radio.write(&gps.location.lat(), sizeof(gps.location.lat()));
    radio.write(&gps.location.lng(), sizeof(gps.location.lng()));
    radio.write(&gps.altitude.meters(), sizeof(gps.altitude.meters()));
    radio.write(&gps.date.month(), sizeof(gps.date.month()));
    radio.write(&gps.date.day(), sizeof(gps.date.day()));
    radio.write(&gps.date.year(), sizeof(gps.date.year()));
    radio.write(&gps.time.hour(), sizeof(gps.time.hour()));
    radio.write(&gps.time.minute(), sizeof(gps.time.minute()));
    radio.write(&gps.time.second(), sizeof(gps.time.second()));

    // Print message to indicate successful transmission
    Serial.println("Alert transmitted successfully.");
  }

  // Add delay for stability
  delay(1000);
}

void displayInfo() {
  if (gps.location.isValid()) {
    Serial.print(F("Location: "));
    Serial.print(gps.location.lat(), 6);
    Serial.print(F(","));
    Serial.println(gps.location.lng(), 6);
    Serial.print(F("Altitude: "));
    Serial.print(gps.altitude.meters());
    Serial.println(F("m"));
  } else {
    Serial.println(F("Location: NOT FOUND"));
  }

  if (gps.date.isValid()) {
    Serial.print(F("Date: "));
    Serial.print(gps.date.month());
    Serial.print(F("/"));
    Serial.print(gps.date.day());
    Serial.print(F("/"));
    Serial.println(gps.date.year());
  }

  if (gps.time.isValid()) {
    Serial.print(F("Time: "));
    Serial.print(gps.time.hour());
    Serial.print(F(":"));
    Serial.print(gps.time.minute());
    Serial.print(F(":"));
    Serial.println(gps.time.second());
  }

  Serial.println();
}
