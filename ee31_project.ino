#define buttonPin 4

// 210
#define bit0 3
#define bit1 2
#define bit2 1

enum states {
  state1, state2, state3, state4, state5, state6, state7
};

enum states buttonState = state1;
enum states nextState;

int lastRead = LOW;
bool buttonPressed = false;
int currRead;

void setup() {
  Serial.begin(9600);
  Serial.println("serial begun");
  pinMode(buttonPin , INPUT); //button input

  pinMode(bit0, OUTPUT);
  pinMode(bit1, OUTPUT);
  pinMode(bit2, OUTPUT);

}

void loop() {
  buttonPressed = false;
  currRead = digitalRead(buttonPin); // CURRENT
  
  if (lastRead == HIGH && currRead == LOW) {
    Serial.println("button released");
    buttonPressed = true;
  } else if (lastRead == LOW && currRead == HIGH) {
    Serial.println("button holding");

  }

  lastRead = currRead;
  delay(1); //readability

  switch (buttonState) {
    case state1: 
      digitalWrite(bit0, HIGH);
      digitalWrite(bit1, LOW);
      digitalWrite(bit2, LOW);

      if (buttonPressed) {
        buttonState = state2;
      }
      break;

    case state2: 
      digitalWrite(bit0, LOW);
      digitalWrite(bit1, HIGH);
      digitalWrite(bit2, LOW);

      if (buttonPressed) {
        buttonState = state3;
      }
      break;

    case state3:
      digitalWrite(bit0, HIGH);
      digitalWrite(bit1, HIGH);
      digitalWrite(bit2, LOW);

      if (buttonPressed) {
        buttonState = state4;
      }
      break;

    case state4: 
      digitalWrite(bit0, LOW);
      digitalWrite(bit1, LOW);
      digitalWrite(bit2, HIGH);

      if (buttonPressed) {
        buttonState = state5;
      }
      break;

    case state5: 
      digitalWrite(bit0, HIGH);
      digitalWrite(bit1, LOW);
      digitalWrite(bit2, HIGH);

      if (buttonPressed) {
        buttonState = state6;
      }

      break;

    case state6: 
      digitalWrite(bit0, LOW);
      digitalWrite(bit1, HIGH);
      digitalWrite(bit2, HIGH);

      if (buttonPressed) {
        buttonState = state7;
      }

      break;

    case state7: 
      digitalWrite(bit0, HIGH);
      digitalWrite(bit1, HIGH);
      digitalWrite(bit2, HIGH);

      if (buttonPressed) {
        buttonState = state1;
      }

      break;

    default: 
      digitalWrite(bit0, LOW);
      digitalWrite(bit1, LOW);
      digitalWrite(bit2, LOW);

      break;
  }

  Serial.print("button state is ");
  Serial.println(buttonState + 1);

}

