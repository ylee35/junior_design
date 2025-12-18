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
 object = objectDetected();

  // while(1) {
  // object = objectDetected();
  // }

  while (1){
      if (currState == state1) {
        // int sent = false;
        // if (!sent){
        //     sendMessage("State 1 ^-^");
        //     sent = true;
        // }

        //bot 1 signals that it will start
        //wait for bot 1 to post red lane found

        bool signalStart = false;
        int timesReceived = 0;
       

        // Trying to see if we can start
        while(!signalStart) {
          while ((client.parseMessage()) > 0) {
            String message = client.readString();

            Serial.print("Received: ");
            Serial.println(message);

            bool startsWithID = message.startsWith("F79721857DC5");
            bool containsBanned = (message.indexOf("CLIENTID:F79721857DC5") != -1);

            if (startsWithID && !containsBanned) {
              timesReceived++;
            }

            // if((message.indexOf('s') != 1) || (message.indexOf('r') != 1)) {
            // timesReceived++;
            // }

            // if (timesReceived == 2) {
            //   proceedYellow = true;
            // }

            // Check if message contains the specific ID
            // if (timesReceived == 4){
            //   if (message.indexOf(clientID2) != 1) {
            //     // if ((message.indexOf('s') != 1) || (message.indexOf('r') != 1)) {
            //       signalStart = true;
            //       Serial.println("Partner team detected!");
            //       break;   // optional: stop once found
            //     // }
            //   }
            // }
            if (timesReceived == 2) {
              // bool startsWithID = message.startsWith("F79721857DC5");
              // bool containsBanned = (message.indexOf("CLIENTID:F79721857DC5") != -1);

              if (startsWithID && !containsBanned) {
                signalStart = true;
                Serial.println("Partner team detected!");
                // timesReceived++;
                // if(timesReceived == 2){
                //   break;
                // }
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
            while(count < 20){
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

          motions.repositionLeft();
          delay(1700);
          motions.stop();
          delay(100);

          Serial.println("Sending to state 3)");
          currState = state3;
      } else if (currState == state3) {
          sendMessage("State 3 ^-^");
          Serial.println("in state 3");
          digitalWrite(LED_BUILTIN, HIGH);
          int *colors = colorSensed();

          // if both are not red then we just go right

          // go backwards to find red lane
          while (colors[LEFT]!= BLUE) {
              motions.forward();
              colors = colorSensed();
              // delay(2000);
          }
          motions.stop();
          delay(100);
          motions.backward();
          delay(400);
          motions.pivot_c();
          delay(350);
          Serial.print("COLORS IS NOW ");
          Serial.println(colors[RIGHT]);

          // while (colors[RIGHT] == RED) {
          //     motions.backward();
          //     colors = colorSensed();
          // }
          motions.repositionRight();
          delay(550);
          motions.stop();
          delay(100);


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
          // comparisonVal = -95;

          delay(10000);
          // post blue lane found signal to server
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
          delay(1100);
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
        //wait for acknowledgement signal from Bot 1
        bool proceedYellow = false;
        int timesReceived2 = 0;
        while(!proceedYellow) {
          while ((client.parseMessage()) > 0) {
            String message = client.readString();

            Serial.print("Received: ");
            Serial.println(message);

            bool startsWithID = message.startsWith("F79721857DC5");
            bool containsBanned = (message.indexOf("CLIENTID:F79721857DC5") != -1);

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
          delay(1100);
          motions.stop();
          
          comparisonVal = -80;
      
          // wait for home signal from Bot 1
          bool partnerHome = false;
          int timesReceived3 = 0;
          while(!partnerHome) {
            while ((client.parseMessage()) > 0) {
              String message = client.readString();

              Serial.print("Received: ");
              Serial.println(message);

              bool startsWithID = message.startsWith("F79721857DC5");
              bool containsBanned = (message.indexOf("CLIENTID:F79721857DC5") != -1);

              if (startsWithID && !containsBanned) {
                timesReceived3++;
              }

              // timesReceived3++;

              // if (timesReceived == 2) {
              //   proceedYellow = true;
              // }

              // Check if message contains the specific ID
              if (timesReceived3 == 1) {
                if (startsWithID && !containsBanned) {
                  partnerHome = true;
                  Serial.println("Partner team detected!");
                  break;   // optional: stop once found
                }
              }
              // if (message.indexOf(clientID2) != 1) {
              //   partnerHome = true;
              //   Serial.println("Partner team detected!");
              //   break;   // optional: stop once found
              // }
            }
          }
          Serial.print("WE RECEIVED THIS: ");
          Serial.println(timesReceived3);
          comparisonVal = -80;
          laneFollowingStraight(YELLOW);
          Serial.println("OUT OF LANE FOLLOWING YELLOW");
          motions.stop();
          currState = state7;
        }
      } else if (currState == state7) {
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
      } else {
          Serial.println("in else");
          motions.stop();
      }
  }

  while(0) {
      Serial.println("out of the if's");
  }

  while(0){
      Serial.print("TOP currState=");
      Serial.println(currState);
      switch(currState) {
          case state1: // cross to the other side
              // int sent = false;
              // if (!sent){
              //     sendMessage("State 1 ^-^");
              //     sent = true;
              // }
              Serial.println("in state1");
              motions.forward();
              while(count < 20){
                  object = objectDetected();
                  count++;
                  motions.forward();
              }

              object = objectDetected();

              if (object) {
                  motions.stop();
                  // digitalWrite(LED_BUILTIN, HIGH);
                  // Serial.println("Object detected");
                  currState = state2;
                  // Serial.println("Sending to state 2");
                  break;
              } else {
                  motions.forward();
                  digitalWrite(LED_BUILTIN, LOW);
              }
              
              // Serial.println("Sending to state 1");
              currState = state1;
              break;

      case state2: //stop state
          // sendMessage("State 2 ^-^");
          Serial.println("in state 2");
          motions.stop();
          delay(500);

          Serial.println("Sending to state 3)");
          currState = state3;
          break;

      case state3: //find red and pivot
          // sendMessage("State 3 ^-^");
          Serial.println("in state 3");
          digitalWrite(LED_BUILTIN, HIGH);
          int *colors = colorSensed();

          // go backwards to find red lane
          while (colors[RIGHT] != RED) { // CHANGE BACK TO LEFT
              motions.backward();
              colors = colorSensed();
              // delay(2000);
          }
          motions.stop();
          delay(500);

          // right sensor is sensor 2 (right)

          // int rightTurn = 0;
          // while (rightTurn < 100) {
          //     Serial.println("initial turn");
          //     rightTurn++;
          //     motions.pivot_cc();
          // }
  

          while (colors[RIGHT] != RED) {
              Serial.println("in loop");
              motions.pivot_cc();
              colors = colorSensed();
          }

          motions.stop();
          Serial.println("DONE");

          // while ((colors[RIGHT] == RED) || (colors[LEFT] == BLACK)){
          //     motions.pivot_cc();
          //     colors = colorSensed();
          // }

  
          // //pivot until we sense black with the right and red with the left

          // motions.stop();
          // delay(5);

          // //get to red on the right
          // while (colors[RIGHT == BLACK]){
          //     motions.pivot_c();
          //     colors = colorSensed();
          // }
          
          Serial.println("Moving to state four");
          currState = state4;
          Serial.print("Just set currState=");
          Serial.println(currState);
          break;

      case state4: //red lane following
          while (1) {
              Serial.print("hola");
          }
          Serial.println("in state 4");
          motions.stop();
          // laneFollowing(RED);
          currState = state5;
          break;


      // if not red then check left, pivot then check left again, then check right then pivot then check right again
      // if not red then go back and then do that again
          // ------------------------ one sensor ----------------------------
          // while (!object) {
          //     while (colors[RIGHT] != RED) {
          //         Serial.println("first while");
          //         motions.pivot_c();
          //         colors = colorSensed();
          //     }
          //     while(colors[RIGHT] != BLACK) {
          //         Serial.println("second while");
          //         motions.pivot_cc();
          //         colors = colorSensed();
          //     }

          // }



          
          // ----------------------------------------------------------------

          // --- Lane following: follow RED lane on BLACK background using 2 sensors ---
          // Case meanings:
          // 1) R=RED, L!=RED  -> forward (we're on the lane with the right sensor)
          // 2) R!=RED, L=RED  -> pivot CCW (bring lane back under right sensor)
          // 3) R=RED, L=RED   -> forward (wide line / crossing / both seeing red)
          // 4) R!=RED, L!=RED -> lost: pivot in last known direction until reacquired

          // Optional: remember which way we last turned while reacquiring
          // enum TurnDir { TURN_CW, TURN_CCW };
          // TurnDir lastTurn = TURN_CW;   // pick a default that makes sense for your bot

          // const int STEP_MS = 25;       // pulse time; 20–40ms is a good start
          // Serial.println("in state 4");
          // while (true) {
          //     bool object = objectDetected();
          //     if (object) {
          //         Serial.println("stopping");
          //         motions.stop();
          //         currState = state5;
          //         break;
          //     }

          //     int *colors = colorSensed();  // update readings each loop
          //     bool R_red = (colors[RIGHT] == RED);
          //     bool L_red = (colors[LEFT]  == RED);

          //     // ----- FOUR CASES -----
          //     if (R_red && !L_red) {
          //         // Case 1: Right sees red, left doesn't -> go forward
          //         Serial.println("CASE 1: R=RED, L!=RED -> forward");
          //         motions.forward();
          //         delay(STEP_MS);
          //         motions.stop();

          //         // If you want: bias memory that line is on the right side
          //         lastTurn = TURN_CW;
          //     }
          //     else if (!R_red && L_red) {
          //         // Case 2: Left sees red, right doesn't -> pivot CCW to bring line under RIGHT
          //         Serial.println("CASE 2: R!=RED, L=RED -> pivot CCW");
          //         motions.pivot_cc();
          //         delay(STEP_MS);
          //         motions.stop();

          //         lastTurn = TURN_CCW;
          //     }
          //     else if (R_red && L_red) {
          //         // Case 3: both red -> forward (or slow forward)
          //         Serial.println("CASE 3: R=RED, L=RED -> forward");
          //         motions.forward();
          //         delay(STEP_MS);
          //         motions.stop();
          //     }
          //     else { 
          //         // Case 4: neither red -> lost line -> search using lastTurn
          //         Serial.println("CASE 4: R!=RED, L!=RED -> searching");
          //         if (lastTurn == TURN_CW) {
          //             motions.pivot_c();
          //         } else {
          //             motions.pivot_cc();
          //         }
          //         delay(STEP_MS);
          //         motions.stop();
          //     }

          //     // loop repeats: re-check object + colors
          // }

          // sendMessage("State 4 ^-^");
          // Serial.println("in state 4");
          // motions.stop();
          // object = objectDetected();
          
          // while (!object && colors[RIGHT] == RED) {
          //     Serial.println("right is red");
          //     motions.forward();
          //     delay(20);
          //     motions.stop();
          //     colors = colorSensed();
          //     object = objectDetected();
          // }
          // while (!object && colors[RIGHT] != RED){
          //     Serial.println("right is not red");
          //     motions.pivot_c();
          //     delay(20);
          //     motions.stop();
          //     colors = colorSensed();
          //     object = objectDetected();
          // }
          // //  if neither right or left are red
          // // pivot cc
          // // check to see if 
          // if (object){
          //     Serial.println("stopping");
          //     currState = state5;
          //     motions.stop();
          //     break;
          // } 
          

      case state5: // turning to find the yellow lane
          Serial.println("in stae5");
          sendMessage("State 5 ^-^");
          motions.backward();
          delay(1000);
          motions.stop();
          delay(5);

          colors = colorSensed();
          while(colors[RIGHT] == RED || colors[LEFT] == RED){
              motions.pivot_c();
              colors = colorSensed();
          }
          motions.stop();
          delay(5);
          colors = colorSensed();
          while (colors[RIGHT] != YELLOW || colors[LEFT] != YELLOW) {
              motions.forward();
              colors = colorSensed();
          }

          motions.stop();

          while (colors[LEFT] == YELLOW){
              motions.pivot_c();
              colors = colorSensed();
          }
          currState = state6;
          break;

      case state6: // moving in the yellow lane
          sendMessage("State 6 ^-^");
      // reposition to make both sensors on yellow
          colors = colorSensed();
          object = objectDetected();
          while (!object && (colors[RIGHT] == YELLOW && colors[LEFT] != YELLOW)){
              motions.forward();
              object = objectDetected();
              colors = colorSensed();
          }
          while(!object && colors[LEFT] == YELLOW){
              motions.pivot_c();
              object = objectDetected();
              colors = colorSensed();
              
          }
          if(object){
              motions.stop();
              delay(5);
              currState = state7;
              break;
          }
          currState = state6;
          break;

      case state7: //turn and go back home
          sendMessage("State 7 ^-^");
          motions.backward();
          delay(1000);
          colors = colorSensed();
          while(colors[RIGHT] == YELLOW || colors[LEFT] == YELLOW){
              motions.pivot_c();
              colors = colorSensed();
          }
          motions.stop();
          delay(5);

          object = objectDetected();
          bool finished = false;
          while(!object){
              motions.forward();
              object = objectDetected();
          }
          finished = true;
          motions.stop();
          bool sentStop = false;
          while (finished){
              motions.stop();
              if (!sentStop){
                  sendMessage("Stopped :p");
                  sentStop = true;
              }
          }
      }
      // }
  }
}
