const int photoPin = A2;  // Photoresistor pin
const int ledPin = 9;     // LED pin
const int threshold = 450; // Adjust this cutoff value for your lighting 
// lower value means more light

void setup() {
  pinMode(ledPin, OUTPUT);
  Serial.begin(9600);
}

void loop() {
  int lightLevel = analogRead(photoPin);
  Serial.println(lightLevel);

  if (lightLevel < threshold) {
    digitalWrite(ledPin, HIGH); // Turn LED on in the dark
  } else {
    digitalWrite(ledPin, LOW);  // Turn LED off in bright light
  }

  delay(200);
}