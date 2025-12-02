#include <Arduino.h>
#include <ArduinoHttpClient.h>
#include <WiFiNINA.h>
#include <CircularBuffer.hpp>

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
#define RED_LED 12 // CHANGE
#define BLUE_LED 13


#define IR_LED 8
#define PHOTO_TRANS A2
#define PHOTO_DETECTOR A3

#define RED_INDEX 0
#define YELLOW_INDEX 1
#define BLUE_INDEX 2 
#define BLACK_INDEX 3
#define WRONG_INDEX 4

extern CircularBuffer<float, 10> redBuffer;
extern CircularBuffer<float, 10> blueBuffer;


//-----------------------------------------------------------------------------

//------------------------- Motor Control -------------------------------------
// Motor A connections
#define enA 9
#define in1 10
#define in2 11
// Motor B connections
#define enB 3
#define in3 5
#define in4 6

//-----------------------------------------------------------------------------


//----------------------- Server Comms ----------------------------------------
extern bool sentOnce;


#endif


// push infrared LED and detecotr closeer and use bigger resistor val