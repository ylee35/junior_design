
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
//#include <CircularBuffer.hpp>

#ifndef DEF_H
#define DEF_H

//------------------------- WiFi Settings -------------------------------------
extern char ssid[]; //username
extern char pass[]; //password

extern char serverAddress[]; // server address
extern int port; //port

extern WiFiClient wifi; //wifi object
extern WebSocketClient client; // client object
//extern String clientID;  // clientID
extern String clientID; // partner ID
extern int status; // client status
extern int count; // idle counter
//-----------------------------------------------------------------------------

//----------------------- Server Comms ----------------------------------------
extern bool sentOnce;

#endif
