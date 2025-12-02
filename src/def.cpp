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

CircularBuffer<float, 10> redBuffer;
CircularBuffer<float, 10> blueBuffer;

bool sentOnce = 1;

// battery ground is thicc grey