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

char serverAddress[] = "34.28.153.91"; // server address
int port = 80; //port

WiFiClient wifi; //wifi object
WebSocketClient client = WebSocketClient(wifi, serverAddress, port); // client object
String clientID = "4A9EDB0160D5";  // clientID
int status = WL_IDLE_STATUS; // client status
int count = 0; // idle counter
//-----------------------------------------------------------------------------

CircularBuffer<float, 10> redBuffer1;
CircularBuffer<float, 10> redBuffer2;

CircularBuffer<float, 10> blueBuffer1;
CircularBuffer<float, 10> blueBuffer2;

CircularBuffer<float, 5> photoBuffer;
// CircularBuffer<float, 4> buffer4;

bool sentOnce = 1;

// battery ground is thicc grey