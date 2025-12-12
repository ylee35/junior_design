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

bool bot1 = false;
bool bot2 = true;

bool bot1flash = false;
bool bot1sent = false;


void setup() {
    Serial.begin(9600);
    wifiSetup();
    setPinModes();
}

void loop() {
//    client.begin();

//   Serial.println("starting WebSocket client");
//   client.begin();
//   sendMessage(clientID);

  
  // client.beginMessage(TYPE_TEXT);
  // client.print(clientID);
  // client.endMessage();

    // while (client.connected()) {

        motions.set_speeds(500, 500, 500);
        
        // Serial.print(x);
        // Serial.print(",");
        // motions.stop();
        int* colorIndices = colorSensed();
        // Serial.print(colorIndices[0]);
        // Serial.println(colorIndices[1]);

        delete[] colorIndices;
        colorIndices = nullptr;

        motions.forward();
        
        // while(1){
        //     switch(currState) {
        //         case state1: // cross to the other side
        //             Serial.println("in state1");
        //             bool object = objectDetected();
        //             while (!object) {
        //                 motions.forward();
        //                 object = objectDetected();
        //                 Serial.println(object);
        //             }

        //             Serial.println(object);
        //             // currState = state2;
        //             // break;

        //         case state2: //stop state
        //             motions.stop();
        //             delay(1000);

        //             currState = state3;
        //             break;

        //         case state3: // 
        //             object = objectDetected();
        //             while (!object) {
        //                 object = objectDetected();
        //                 int *colors = colorSensed();

        //                 // go backwards to find red lane
        //                 while (colors[RIGHT] != RED && colors[LEFT] != RED) {
        //                     motions.backward();
        //                 }

        //                 // turn roughly 90 degrees 
        //                 int turnTime = 0;
        //                 while (turnTime < 30) { // CALIBRATE this value 
        //                     motions.left_turn();
        //                     turnTime++;
        //                 }
        //                 motions.stop();

            
        //                 // my idea is we turn an elementary 90 degrees then keep turning to make it on the lane? 
        //                 // but also we can see after testing it 

        //             }
        //             currState = state4;
        //             break;
               
        //         // while(left sensor, right sensor not red){
        //         // go backwards
        //         //}
        //         // while either sensor red,
        //         // if left sensor red but not right, pivot towards right until right sensor red
        //         //pivot right until right sensor is black
        //         //when right sensor black, pivot left until right sensor red and left sensor red
        //         //go to state 4

        //         case state4:
        //             // reposition to make both sensors on red 
        //             int *colors = colorSensed();
        //             while (colors[RIGHT] != RED && colors[LEFT] != RED) {
        //                 motions.right_turn();
        //             }
        //             motions.stop();
                    
        //             object = objectDetected();

        //             while (!object) {
        //                 int *colors = colorSensed();
        //                 while (colors[RIGHT] == RED && colors[LEFT] == RED) {
        //                     motions.forward();
        //                     colors = colorSensed();
        //                 }

        //                 while (colors[RIGHT] == BLACK && colors[LEFT] == RED) {
        //                     motions.left_turn();
        //                     colors = colorSensed();
        //                 }

        //                 while (colors[RIGHT] == RED && colors[LEFT] == BLACK) {
        //                     motions.right_turn();
        //                     colors = colorSensed();
        //                 }
        //             }

        //             motions.stop();
        //             currState = state5;
        //             break;
        //         //object polling
        //         // while wall not sensed
        //         // while left and right are red
        //         // go forward
        //         //when left is red and right is black turn to left
        //         //when right is red left is black turn to right
        //         // while wall is sensed, stop and move to state 5 and break
        //         //move to state 5

        //         case state5: // turning to find the yellow lane
        //             int backTime = 0;
        //             while (backTime < 10) { // CALIBRATE LATER
        //                 motions.backward();
        //             }
        //             motions.stop();

        //             int *colors = colorSensed();
        //             while (colors[RIGHT] != BLACK || colors[LEFT] != BLACK) {
        //                 motions.left_turn();
        //                 colors = colorSensed();
        //             }
        //             motions.stop();

        //             colors = colorSensed();
        //             while (colors[RIGHT] != YELLOW || colors[LEFT] != YELLOW) {
        //                 motions.forward();
        //             }

        //             motions.stop();
        //             currState = state6;
        //             break;                    
                    
        //         //go backward slightly to have room to turn
        //         //turn to left until both sense black
        //         //go forward until both sense yellow
        //         //go forward until both sense black
        //         //turn left until both sense yellow
        //         //turn left until left senses black
        //         //turn right until both sense yellow
        //         //go to state 6, break

        //         case state6: // moving in the yellow lane
        //             // reposition to make both sensors on yellow 
        //             int *colors = colorSensed();
        //             while (colors[RIGHT] != YELLOW && colors[LEFT] != YELLOW) {
        //                 motions.left_turn();
        //             }
        //             motions.stop();
                    
        //             object = objectDetected();

        //             while (!object) {
        //                 int *colors = colorSensed();
        //                 while (colors[RIGHT] == YELLOW && colors[LEFT] == YELLOW) {
        //                     motions.forward();
        //                     colors = colorSensed();
        //                 }

        //                 while (colors[RIGHT] == BLACK && colors[LEFT] == YELLOW) {
        //                     motions.left_turn();
        //                     colors = colorSensed();
        //                 }

        //                 while (colors[RIGHT] == RED && colors[LEFT] == YELLOW) {
        //                     motions.right_turn();
        //                     colors = colorSensed();
        //                 }
        //             }

        //             motions.stop();
        //             currState = state7;
        //             break;
                    
        //         // bool object = objectDetected();
        //         // while(object){
        //         // motions.stop();
        //         // bool object = objectDetected();
        //         // Serial.println(object);
        //         // currState = state7;
        //         // break;
        //         // }
        //         // while(!object){
        //         // motions.forward();
        //         // bool object = objectDetected();
        //         // Serial.println(object);
        //         // }
        //         // currState = state7;

        //         default: // go back to where we started
        //             int turnTime = 0;
        //             while (turnTime < 10) { // CHANGE THIS VALUE
        //                 motions.left_turn();
        //                 turnTime++;
        //             }

        //             object = objectDetected();
        //             while (!object) {
        //                 motions.forward();
        //                 object = objectDetected();
        //             }
        //             motions.stop();
        //         //turn right until both sense black
        //         //go forward until wall detected
        //             break;
        //     }
        // }
}

