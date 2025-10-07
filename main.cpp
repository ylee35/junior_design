#include "functions.h"
#include "def.h"


enum states{state1 , state2 , state3 , state4 , state5 , state6 , state7};
// state 1 : cross to other side
// state 2: Stop when it senses the wall at the top and turn around
// state 3. Cross back to find the red lane,
// state 4. Follow the red lane until it senses the wall at the right
// state 5. Turn left and find the yellow lane
// state 6. Follow the yellow lane until it senses the wall at the left
// state 7. Turn left and return to the starting position
enum states currState = state1;


void setup() {
  Serial.begin(9600);
  // while ( status != WL_CONNECTED) {
  //   Serial.print("Attempting to connect to Network named: ");
  //   Serial.println(ssid); // print the network name (SSID);

  //   // Connect to WPA/WPA2 network:
  //   status = WiFi.begin(ssid, pass);
  // }

  // // print the SSID of the network you're attached to:
  // Serial.print("SSID: ");
  // Serial.println(WiFi.SSID());

  // // print your WiFi shield's IP address:
  // IPAddress ip = WiFi.localIP();
  // Serial.print("IP Address: ");
  // Serial.println(ip);

  pinMode(RED_LED , OUTPUT);
  pinMode(BLUE_LED, OUTPUT);
  pinMode(YELLOW_LED, OUTPUT);
  pinMode(IR_LED, OUTPUT);
  pinMode(PHOTO_TRANS, INPUT);
  pinMode(PHOTO_DETECTOR , INPUT);
}

void loop() {
  // client.begin();

  // if (client.connected()) {
  //   //parseMessage and readString

  //   int messageReceived = client.parseMessage();
  //   if (messageReceived) {
  //       String parsed = parsedMessage(messageReceived);
  //       Serial.println(parsed);
  //   }
  // }
  
  // int colorIndex = colorSensed();
  bool object = objectDetected();
  // Serial.println(object);
  // switch(currState){
  //   case state1: // cross to the other side
  //     // while wall not sensed, 
  //     // go forward
      
  //     // go to state
  //     //wall sensed, go to state 2
  //   case state2: 
  //     /*
  //     stop      
  //     */
  //   case state3:
  //     // while lane not red
  //     // go backwards
      
  //     //when lane red
  //     // stop and go to state 4
  //   case state4:
  //     // while right wall not sensed 
  //       // while lane is red
  //         // go forward 
  //   case state5:

  //   case state6:
    
  //   case state7:
    
  // }



}

