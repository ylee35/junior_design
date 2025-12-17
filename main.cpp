#include "def.h"
#include "functions.h"

void setup() {
  lcd.begin(16,2);
  Serial.begin(9600);
  // Print a message to the LCD.
  Serial.println("printing in setup");
  lcd.print("hello");
  lcd.setCursor(0, 2);
  lcd.print("world");

  wifiSetup();

  Serial.println("starting WebSocket client");
  client.begin();

  // identify client ONCE
  client.beginMessage(TYPE_TEXT);
  client.print(clientID);
  client.endMessage();
}

unsigned long lastSend = 0;

void loop() {
  if (!client.connected()) {
    Serial.println("WebSocket disconnected");
    return;
  }

  // Send message every 1 second
  if (millis() - lastSend > 1000) {
    lastSend = millis();
    client.beginMessage(TYPE_TEXT);
    client.print("Campbell");
    client.endMessage();

    Serial.println("Message sent to server");
  }

  //Drain incoming messages
  int messageSize;
  while ((messageSize = client.parseMessage()) > 0) {
    String message = client.readString();

    Serial.print("Received: ");
    Serial.println(message);

    lcd.clear();
    lcd.print(message);
  }

  delay(10); // needed delay for WiFiNINA

}