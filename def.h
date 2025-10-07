#include <Arduino.h>
#include <ArduinoHttpClient.h>
#include <WiFiNINA.h>

#ifndef DEF_H
#define DEF_H

//------------------------- WiFi Settings -------------------------------------
extern char ssid[]; //username
extern char pass[]; //password

extern char serverAddress[]; // server address
extern int port; //port

extern WiFiClient wifi; //wifi object
extern WebSocketClient client; // client object
extern String clientID;  // clientID
extern int status; // client status
extern int count; // idle counter
//-----------------------------------------------------------------------------
//------------------------- Color Sensor --------------------------------------
#define RED_LED 9
#define BLUE_LED 10
#define YELLOW_LED 11
#define IR_LED 12
#define PHOTO_TRANS A2
#define PHOTO_DETECTOR A3

#define RED_INDEX 0
#define YELLOW_INDEX 1
#define BLUE_INDEX 2 

//-----------------------------------------------------------------------------
#endif