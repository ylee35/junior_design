#include "functions.h"

String parsedMessage(int messageReceived){
  String message = client.readString();
  // Serial.print("message is ");
  // Serial.println(message);
  String parsed;

  if (message.substring(0, 22) == "WebClient_4A9EDB0160D5") {
    parsed = message.substring(23, messageReceived);
  }

  return parsed;
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


