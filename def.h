#include <Arduino.h>
#include <ArduinoHttpClient.h>
#include <WiFiNINA.h>
#include <LiquidCrystal.h>

#ifndef DEF_H
#define DEF_H

//------------------------- WiFi Settings -------------------------------------
extern char ssid[]; //username
extern char pass[]; //password

extern char serverAddress[]; // server address
extern int port; //port

extern WiFiClient wifi; //wifi object
extern WebSocketClient client; // client object
extern WebSocketClient client2;
extern String clientID;  // clientID
extern String clientID2;
extern int status; // client status
extern int count; // idle counter
//-----------------------------------------------------------------------------

//-------------------- LCD Settings -------------------------------------------
extern const int rs, en, d4, d5, d6, d7;
extern LiquidCrystal lcd;
#endif