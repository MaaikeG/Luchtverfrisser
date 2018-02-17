const int freshenerPin = 12;
const int overridePin = 14;

void setup() {
  pinMode(freshenerPin, OUTPUT);
  pinMode(overridePin, INPUT);
}

void loop() {
  int buttonState = digitalRead(overridePin);
  
  if (buttonState == HIGH) {
    digitalWrite(freshenerPin, HIGH);
  } else {
    digitalWrite(freshenerPin, LOW);
  }
}
