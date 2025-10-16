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
String parsed;

BotMotions motions;

bool bot1 = true;
bool bot2 = false;

bool bot1flash = false;
bool bot1sent = false;

bbool bot2flash = false;
bool bot2setn = false;

void setup() {
  Serial.begin(9600);
  wifiSetup();
  setPinModes();
  
  //delete later
  pinMode(LED_BUILTIN , OUTPUT);
  
}

void loop() {
  // client.begin();

  Serial.println("starting WebSocket client");
  client.begin();
  client.beginMessage(TYPE_TEXT);
  client.print(clientID);
  client.endMessage();

  while (client.connected()) {
    // String *message;
    // bool receivedPls = receiveMessage(message); // might not be 
    // Serial.print("Parsed is ");
    // Serial.println(*message);

    int messageReceived = client.parseMessage();
    if (messageReceived) {
      String message = client.readString();

      if (message.substring(0, 22) == "WebClient_4A9EDB0160D5") {
        parsed = message.substring(23, messageReceived);
        Serial.print("2parsed is ");
        Serial.println(parsed);
      }
    }

  //////////////// REMOTELY COMMANDED BOT MOTIONS (TEMPORARY) /////////////////

  /*  Both bots are next to each other and not moving.
      Bot 1 flashes their Arduino LED.
      Bot 1 communicates to Bot 2 to move forward for five seconds. 
      Bot 2 receives the message and move forward for five seconds then stops.
      When Bot 2 is finished moving forward, it flashes it's built-in LED. 
      Bot 2 communicates to Bot 1 to move forward for five seconds. 
      Bot 1 receives the signal and moves forward for five seconds then stop
  */
  
  while (bot1) {
      int messageReceived = client.parseMessage();
      if (messageReceived) {
        String message = client.readString();

        if (message.substring(0, 22) == "WebClient_4A9EDB0160D5") {
          parsed = message.substring(23, messageReceived);
          Serial.print("2parsed is ");
          Serial.println(parsed);
        }
      }
      
      if (!bot1flash){
        Serial.println("in bot 1");
        digitalWrite(LED_BUILTIN, HIGH);
        delay(2000);
        digitalWrite(LED_BUILTIN, LOW);
        bot1flash = true;
      }


      if (!bot1sent){
        client.beginMessage(TYPE_TEXT);
        client.print("bot 2 can go");
        client.endMessage();
        bot1sent = true;
      }

      if (parsed == "bot 1 can go NOW"){
        Serial.println("we can go"); // testing 
        motions.forward();
        Serial.println("TEST: moving forward");
        delay(5000);
        motions.stop();
        Serial.println ("TEST: stoppped");
        bot1 = false; 
        bot2 = true;
      }
    }

    while (bot2) {
      Serial.println("in bot 2");

      int messageReceived = client.parseMessage();
      if (messageReceived) {
        String message = client.readString();

        if (message.substring(0, 22) == "WebClient_4A9EDB0160D5") {
          parsed = message.substring(23, messageReceived);
          Serial.print("2parsed is ");
          Serial.println(parsed);
        }
      }

      if (parsed == "bot 2 can go from bot 1"){
        motions.forward();
        Serial.println("TEST: moving forward");

        delay(5000);
        Serial.println ("TEST: stoppped");
        motions.stop();
        
        digitalWrite(LED_BUILTIN, HIGH);
        delay(2000);
        digitalWrite(LED_BUILTIN, LOW);

        client.beginMessage(TYPE_TEXT);
        client.print("bot 1 can go");
        client.endMessage();
      } 
      bot2 = false;
    }

    
    if (sentOnce) {
      String messageToTeam = "hi team";
      sendMessage(messageToTeam);
    }

  }

  ///////////////////////////////////////////////////////////////////////////
  
  // int colorIndex = colorSensed();
  // bool object = objectDetected();
  // Serial.println(object);
  
  // switch(currState){
  //   case state1: // cross to the other side
  //     // while wall not sensed, 
  //     // go forward
  //     motions.forward();
  //     delay(5000);
  //     // go to state
  //     //wall sensed, go to state 2
  //   case state2: 
  //     /*
  //     stop      
  //     */
  //    motions.stop();
  //    delay(1000);
  //   case state3:
  //     // while lane not red
  //     // go backwards
  //     motions.backward();
  //     delay(5000);
  //     //when lane red
  //     // stop and go to state 4
  //     motions.stop();
  //     delay(1000);
  //     motions.right_turn();
  //     delay(1000);
  //   case state4:
  //     // while right wall not sensed 
  //       // while lane is red
  //         // go forward 
  //     motions.forward();
  //     delay(5000);
  //     motions.stop();
  //     delay(1000);
  //   case state5:
  //     motions.left_turn();
  //     delay(1000);
  //     motions.stop();
  //     delay(1000);
  //   //turn left
  //   //find yellow lane
  //   case state6:
  //     motions.forward();
  //     delay(5000);
  //     motions.stop();
  //     delay(1000);
  //   //while wall not sensed
  //   //follow yellow
  //   case state7:
  //     motions.left_turn();
  //     delay(1000);
  //     motions.stop();
  //     delay(1000);
  //     motions.forward();
  //     delay(5000);
  //     motions.stop();
  //     delay(1000);
  //   //turn left
  //   //go forward to starting position
  //}



}

