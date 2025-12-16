
/*
 *      def.h
 * 
 *      This file contains includes to the libraries we use and declarations of
 *      the WiFi setting parameters. It also has definitions for pin connections, 
 *      color indices, and buffers for color and object sensing, as well as pin connections
 *      for the motors. 
 * 
*/

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
#define RED_LED 12 
#define BLUE_LED 13

#define IR_LED 8
#define PHOTO_TRANS_1 A2 // RIGHT
#define PHOTO_TRANS_2 A3 // LEFT
#define PHOTO_DETECTOR A4

#define RED 0
#define YELLOW 1
#define BLUE 2 
#define BLACK 3
#define WRONG 4

#define RIGHT 0
#define LEFT 1

#define BUTTON_PIN 7

extern CircularBuffer<float, 5> redBuffer1;
extern CircularBuffer<float, 5> redBuffer2;

extern CircularBuffer<float, 5> blueBuffer1;
extern CircularBuffer<float, 5> blueBuffer2;

extern CircularBuffer<float, 5> photoBuffer;
// CircularBuffer<float, 4> buffer4;


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