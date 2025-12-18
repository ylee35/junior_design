#include "functions.h"
#include "def.h"
#include "colorSensing.h"
#include "motions.h"

enum states{state1 , state2 , state3 , state4 , state5 , state6 , state7};
// state 1 : cross to other side
// state 2: Stop when it senses the wall at the top and turn around
// state 3. Cross back to find the blue lane,
// state 4. Follow the blue lane until it senses the wall at the right
// state 5. Turn left and find the yellow lane
// state 6. Follow the yellow lane until it senses the wall at the left
// state 7. Turn left and return to the starting position
enum states currState = state1;
String parsed;

BotMotions motions;

bool bot1 = false;
bool bot2 = true;

bool bot1flash = false;
bool bot1sent = false;


void setup() {
  Serial.begin(9600);
 
  setPinModes();

  wifiSetup();

  Serial.println("starting WebSocket client");
  client.begin();

  // identify client ONCE
  client.beginMessage(TYPE_TEXT);
  client.print(clientID);
  client.endMessage();
}

void loop() {
//------------------------------------ Server Communication -----------------------------------
  if (!client.connected()) {
    Serial.println("WebSocket disconnected");
    return;
  }

  // Send message every 1 second
  //if (millis() - lastSend > 1000) {
    //lastSend = millis();
    client.beginMessage(TYPE_TEXT);
    client.print("Campbell");
    client.endMessage();

    Serial.println("Test message sent to server");
  //}

  //Drain incoming messages
  // int messageSize;
  while ((client.parseMessage()) > 0) {
    String message = client.readString();

    Serial.print("Received: ");
    Serial.println(message);

  }

  delay(10); // needed delay for WiFiNINA
//-------------------------------------------------------------------------------------------
    calibrateAllColors();

    // Serial.println("starting WebSocket client");
    // client.begin();
    // sendMessage(clientID);

    // client.beginMessage(TYPE_TEXT);
    // client.print(clientID);
    // client.endMessage();

    // while (client.connected()) {
    // Serial.print(x);
    // Serial.print(",");
    // motions.stop();
    int* colorIndices = colorSensed();

    delete[] colorIndices;
    colorIndices = nullptr;

    // motions.forward();
    // bool object = objectDetected();
    // if (object) {
    // motions.stop();
    // digitalWrite(LED_BUILTIN, HIGH);
    // } else {
    // motions.forward();
    // digitalWrite(LED_BUILTIN, LOW);
    // }
    // bool object = objectDetected();

    // while(1) {
    // bool object = objectDetected();
    // while (!object) {
    // motions.forward();
    // }
    // motions.stop();
    // }
    //motions.forward();
    bool object = false;
    int count = 0;

    motions.set_speeds(150, 150, 150);

    while (1) {
        if (currState == state1) {
            // int sent = false;
            // if (!sent){
            //     sendMessage("State 1 ^-^");
            //     sent = true;
            // }
            bool signalStart = false;
            int timesReceived = 0;

            // Trying to see if we can start
            while(!signalStart){
              while((client.parseMessage()) > 0) {
                String message = client.readString();

                Serial.print("Received: ");
                Serial.println(message);

                bool startsWithID = message.startsWith("56FC703ACE1A");
                bool containsBanned = (message.indexOf("CLIENTID:56FC703ACE1A") != -1);

                if (startsWithID && !containsBanned) {
                  timesReceived++;
                }
                if (timesReceived == 2) {
                  if (startsWithID && !containsBanned) {
                    signalStart = true;
                    Serial.println("Partner team detected!");
                    break;
                  }
                }


              }
            }
            Serial.print("WE RECEIVED THIS MANY TIMES: ");
            Serial.println(timesReceived);

            if (signalStart) {
              // Letting partner team know we are about to start
              client.beginMessage(TYPE_TEXT);
              client.print("About to start");
              client.endMessage();

              Serial.println("Message sent to partner team");
              delay(10);

              // actually starting

              Serial.println("in state1");
              motions.forward();

              while (1) {
                while(count < 20) {
                  object = objectDetected();
                  count++;
                  motions.forward();
                }

                object = objectDetected();

                if (object) {
                    motions.stop();
                    delay(100);
                    // digitalWrite(LED_BUILTIN, HIGH);
                    // Serial.println("Object detected");
                    currState = state2;
                    break;
                    // Serial.println("Sending to state 2");
                } else {
                    motions.forward();
                    digitalWrite(LED_BUILTIN, LOW);
                }
              }
              
            


            }

            

        } else if (currState == state2) {
            sendMessage("State 2 ^-^");
            Serial.println("in state 2");
            motions.stop();
            delay(100);

            motions.repositionLeft(); // TURN TO GO BACK TO BLUE
            delay(1400);
            motions.stop();
            delay(100);

            Serial.println("Sending to state 3)");
            currState = state3;
        } else if (currState == state3) {
            sendMessage("State 3 ^-^");
            Serial.println("in state 3");
            digitalWrite(LED_BUILTIN, HIGH);
            int *colors = colorSensed();

            // if both are not red then we just go riht

            // go backwards to find red lane
            while (colors[LEFT] != BLUE) { // CHANGE BACK TO LEFT
              motions.forward();
              colors = colorSensed();
              // delay(2000);
            }
            motions.stop();
            delay(100);
            // motions.backward();
            // delay(400);
            motions.pivot_c();
            delay(350);
            Serial.print("COLORS IS NOW ");
            Serial.println(colors[LEFT]);

            // while (colors[RIGHT] == RED) {
            //     motions.backward();
            //     colors = colorSensed();
            // }

            // THIS IS NOT COMMENTED OUT IN THE SOLO CODE
            motions.repositionRight();
            delay(400);
            motions.stop();
            delay(100);// 10 11 104 107


            // while (colors[RIGHT] != RED) {
            //     Serial.println("in loop");
            //     motions.pivot_cc();
            //     colors = colorSensed();
            // }

            Serial.println("DONE");

            Serial.println("Moving to state four");
            currState = state4;
            Serial.print("Just set currState=");
            Serial.println(currState);
        } else if (currState == state4) {
            Serial.println("in state 4");
            motions.stop();
            delay(100);
            comparisonVal = -90;

            delay(10000);
            //post blue lane found signal to server
            client.beginMessage(TYPE_TEXT);
            client.print("blue lane found");
            client.endMessage();

            Serial.println("Message sent to partner team");
            delay(10);

            laneFollowing(BLUE);
            currState = state5;
        } else if (currState == state5) {
            Serial.println("in state 5");
            motions.repositionRight();
            delay(900);
            motions.stop();
            delay(100);
            int *colors = colorSensed();

            while (colors[RIGHT] != YELLOW) {
                motions.forward();
                colors = colorSensed();
            }

            motions.stop();
            delay(100);

            currState = state6;
        } else if (currState == state6) {
          // wait for acknoledgment signal from Bot 1
            bool proceedYellow = false;
            int timesReceived2 = 0;
            while(!proceedYellow) {
              while ((client.parseMessage()) > 0) {
                String message = client.readString();

                Serial.print("Received: ");
                Serial.println(message);

                bool startsWithID = message.startsWith("56FC703ACE1A");
                bool containsBanned = (message.indexOf("CLIENTID:56FC703ACE1A") != -1);

                if (startsWithID && !containsBanned) {
                  timesReceived2++;
                }

                // timesReceived2++;

                // if (timesReceived == 2) {
                //   proceedYellow = true;
                // }

                // Check if message contains the specific ID
                if (timesReceived2 == 1) {
                  if (startsWithID && !containsBanned) {
                    proceedYellow = true;
                    Serial.println("Partner team detected!");
                    break;   // optional: stop once found
                  }
                }
                // if (startsWithID && !containsBanned) {
                //   proceedYellow = true;
                //   Serial.println("Partner team detected!");
                //   break;   // optional: stop once found
                // }
              }
            }
            Serial.print("WE RECEIVED THIS: ");
            Serial.println(timesReceived2);

            if (proceedYellow) {
              Serial.println("in state 6");
              int *colors = colorSensed();
              while (colors[RIGHT] != BLACK) {
                  motions.forward();
                  colors = colorSensed();
              }

            motions.repositionRight();
            delay(900);
            motions.stop();
            
            comparisonVal = -80;
            
            bool partnerHome = false;
            int timesRecevied3 = 0;
            while (!partnerHome){
              while ((client.parseMessage()) > 0) {
                String message = client.readString();

                Serial.print("Received: ");
                Serial.println(message);

                bool startsWithID = message.startsWith("56FC703ACE1A");
                bool containsBanned = (message.indexOf("CLIENTID:56FC703ACE1A") != -1);

                if (startsWithID && !containsBanned) {
                  timesRecevied3++;
                }

                // Check if message contains the specific ID
                if (timesRecevied3 == 1) {
                  if (startsWithID && !containsBanned) {
                    partnerHome = true;
                    Serial.println("Partner team home detected!");
                    break;   // optional: stop once found
                  }
                }
              }
            }
            Serial.print("WE RECEIVED THIS: ");
            Serial.println(timesRecevied3);

            Serial.println("Starting lane following yellow");

            laneFollowingStraight(YELLOW);
            Serial.println("OUT OF LANE FOLLOWING YELLOW");
            motions.stop();
            // currState = state7;
            Serial.println("in state 7");
            motions.repositionRight();
            delay(1000);
            motions.stop();

            motions.forward();
            delay(750);
            motions.stop();

            while (1) {
              motions.stop();
            }
        } 
        else if (currState == state7) {
            Serial.println("in state 7");
            motions.repositionRight();
            delay(1000);
            motions.stop();

            // comparisonVal = -85;

            // object = objectDetected();

            // while (!object) {
            //     motions.forward();
            //     object = objectDetected();
            // }

            motions.forward();
            delay(750);
            motions.stop();

            while (1) {
              motions.stop();
            }
        } 
        else {
            Serial.println("in else");
            motions.stop();
        }
    }
}
}
