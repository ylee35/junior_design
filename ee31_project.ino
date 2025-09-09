int buttonPin = 4;

void setup() {
  Serial.begin(9600);
  Serial.println("serial begun");
  pinMode (buttonPin , INPUT); //button input
}

void loop() {
  int buttonState = digitalRead(buttonPin); //state of button
  Serial.println(buttonState); //print state
  delay(1); //readability
}
