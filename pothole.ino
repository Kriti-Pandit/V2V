
void setup() {
  Serial.begin(9600);
}

void loop() {
  if (Serial.available() > 0) {
    char signal = Serial.read();
    // Perform actions based on received signals
    if (signal == '1') {
      // Example action: print a message to the Serial monitor
      Serial.println("Pothole detected!");
      // You can add other actions here
    }
  }
}