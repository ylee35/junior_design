#include "functions.h"

String receiveMessage() {
  int messageReceived = client.parseMessage();
  while (!messageReceived) {    
    messageReceived = client.parseMessage();
  }

  String parsed;
  if (messageReceived) {
    String message = client.readString();
    for (int i = 0 ; i <= (messageReceived - 12); i++){
      if (message.substring(i , i + 12) == "828BD9E1B7C7"){
        for (int j = i + 12; j <= messageReceived; j++){
          if (message.substring(j , j + 1) == "."){
            parsed = message.substring(j + 1 , messageReceived);
            break;
          }
        }
        break;
      }
    }
  } 

  return parsed;
}

void sendMessage(String messageToTeam){
  client.beginMessage(TYPE_TEXT);
  client.print(messageToTeam);
  client.endMessage();
  Serial.println("sent");
  sentOnce = false;
}

int colorSensed() {
  // TODO REMOVE DELAYS FOR FINAL VERSION
  
  /* check red */
  // turn red LED on, yellow off
  digitalWrite(YELLOW_LED, LOW);
  digitalWrite(RED_LED, HIGH);
  Serial.println("after red");
  // read from sensor
  float vout1 = analogRead(PHOTO_TRANS);
  Serial.print("red led is ");
  Serial.println(vout1);
  delay(100);

  /* check blue */
  // turn red off, blue on 
  digitalWrite(RED_LED , LOW);
  digitalWrite(BLUE_LED , HIGH);

  // read from sensor
  float vout2 = analogRead(PHOTO_TRANS);
  Serial.print("blue led is: ");
  Serial.println(vout2);
  delay(100);

  /* check yellow */
  // turn blue off, yellow on 
  digitalWrite(BLUE_LED, LOW);
  digitalWrite(YELLOW_LED, HIGH);

  // read from sensor 
  float vout3 = analogRead(PHOTO_TRANS);
  Serial.print("yellow led is: ");
  Serial.println(vout3);
  delay(100);


  /* determine color */ 
  // TODO MORGAN AND KAYLA AFTER BATTERY AND HEIGHT FROM GROUND DETERMINED

}

bool objectDetected() {
  digitalWrite(IR_LED, HIGH);
  float photodetector = analogRead(PHOTO_DETECTOR);
  Serial.print("photodetector is: ");
  Serial.println(photodetector);
  delay(100);

  if (photodetector > 900) {
    return true;
  }

  return false;
}

void BotMotions::stop() {
  // Turn off motors
  digitalWrite(in1, LOW);
  digitalWrite(in2, LOW);
  digitalWrite(in3, LOW);
  digitalWrite(in4, LOW);
}

void BotMotions::backward() {
  digitalWrite(enA, HIGH);
  digitalWrite(enB, HIGH);
  // Turn on motors
  digitalWrite(in1, LOW); //right motor
  digitalWrite(in3, LOW); //left motor
  digitalWrite(in2, HIGH); //right motor
  digitalWrite(in4, HIGH); //left motor

  analogWrite(enB, max_speed); //left motor enable
  analogWrite(enA, max_speed); //right motor enable
  delay(20);
  
}

void BotMotions::forward() {
  Serial.println("in forward");
  digitalWrite(enA, HIGH);
  digitalWrite(enB, HIGH);
  // Turn on motors
  digitalWrite(in1, HIGH); //right motor
  digitalWrite(in3, HIGH); //left motor
  digitalWrite(in2, LOW); //right motor
  digitalWrite(in4, LOW); //left motor

  analogWrite(enB, max_speed);
  analogWrite(enA, max_speed);
  delay(20);
}

void BotMotions::pivot_c() {
  digitalWrite(enA, HIGH);
  digitalWrite(enB, HIGH);
  // Turn on motors
  digitalWrite(in1, LOW); //right motor
  digitalWrite(in3, HIGH); //left motor
  digitalWrite(in2, LOW); //right motor
  digitalWrite(in4, LOW); //left motor

  analogWrite(enB, max_speed);
  analogWrite(enA, max_speed);
  delay(20);

}

void BotMotions::pivot_cc() {
  digitalWrite(enA, HIGH);
  digitalWrite(enB, HIGH);
  // Turn on motors
  digitalWrite(in1, HIGH); //right motor
  digitalWrite(in3, LOW); //left motor
  digitalWrite(in2, LOW); //right motor
  digitalWrite(in4, LOW); //left motor

  analogWrite(enB, max_speed);
  analogWrite(enA, max_speed);
  delay(20);

}

void BotMotions::right_turn() {
  //LEFT SPEED HAS TO BE SET HIGHER

  digitalWrite(enA, HIGH);
  digitalWrite(enB, HIGH);
  // Turn on motors
  digitalWrite(in1, HIGH); //right motor
  digitalWrite(in3, HIGH); //left motor
  digitalWrite(in2, LOW); //right motor
  digitalWrite(in4, LOW); //left motor


  analogWrite(enB, left_speed);
  analogWrite(enA, right_speed);
  delay(20);

}

void BotMotions::left_turn() {
  //RIGHT SPEED HAS TO BE HIGHER
  //Note: left motor is weak, so we need to have at least 50

  digitalWrite(enA, HIGH);
  digitalWrite(enB, HIGH);
  // Turn on motors
  digitalWrite(in1, HIGH); //right motor
  digitalWrite(in3, HIGH); //left motor
  digitalWrite(in2, LOW); //right motor
  digitalWrite(in4, LOW); //left motor


  analogWrite(enB, left_speed);
  analogWrite(enA, right_speed);
  delay(20);

}

void BotMotions::set_speeds(int left_speed, int right_speed, int max_speed) {
  this->left_speed = left_speed;
  this->right_speed = right_speed;
  this->max_speed = max_speed;
}

void setPinModes() {
  /* Color Detection and Object Detection I/O */
  pinMode(RED_LED , OUTPUT);
  pinMode(BLUE_LED, OUTPUT);
  pinMode(YELLOW_LED, OUTPUT);
  pinMode(IR_LED, OUTPUT);
  pinMode(PHOTO_TRANS, INPUT);
  pinMode(PHOTO_DETECTOR , INPUT);

  /* Motor Control I/O */
  pinMode(enA, OUTPUT);
  pinMode(enB, OUTPUT);
  pinMode(in1, OUTPUT);
  pinMode(in2, OUTPUT);
  pinMode(in3, OUTPUT);
  pinMode(in4, OUTPUT);

  /* Turn off motors - Initial state */ 
  digitalWrite(in1, LOW);
  digitalWrite(in2, LOW);
  digitalWrite(in3, LOW);
  digitalWrite(in4, LOW);
  digitalWrite(enA, LOW);
  digitalWrite(enB, LOW);
}

void wifiSetup() {
  while (status != WL_CONNECTED) {
    Serial.print("Attempting to connect to Network named: ");
    Serial.println(ssid); // print the network name (SSID);

    // Connect to WPA/WPA2 network:
    status = WiFi.begin(ssid, pass);
  }

  // print the SSID of the network you're attached to:
  Serial.print("SSID: ");
  Serial.println(WiFi.SSID());

  // print your WiFi shield's IP address:
  IPAddress ip = WiFi.localIP();
  Serial.print("IP Address: ");
  Serial.println(ip);
}