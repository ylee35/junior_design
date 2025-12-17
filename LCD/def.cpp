/*
 *      def.cpp
 * 
 *      This file contains definitions for the WiFi parameters and declarations
 *      for the bfufers used in color sensing. 
 * 
*/

#include "def.h"

//------------------------- WiFi Settings -------------------------------------
char ssid[] = "tufts_eecs"; //username
char pass[] = "foundedin1883"; //password

//char serverAddress[] = "10.50.42.66"; // our server address
char serverAddress[] = "35.239.140.61"; // Halligan server
//int port = 8765; // our port
int port = 8080; // Halligan port

WiFiClient wifi; //wifi object
WebSocketClient client = WebSocketClient(wifi, serverAddress, port); // client object
//String clientID = "4A9EDB0160D5";  // clientID
String clientID = "56FC703ACE1A"; //clientID
int status = WL_IDLE_STATUS; // client status
int count = 0; // idle counter
//-----------------------------------------------------------------------------
const int rs = 12, en = 11, d4 = 5, d5 = 4, d6 = 3, d7 = 2;
LiquidCrystal lcd(rs, en, d4, d5, d6, d7);
