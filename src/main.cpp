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

        // motions.set_speeds(0 , 0 , 200);
        
        // Serial.print(x);
        // Serial.print(",");
        int* colorIndices = colorSensed();
        Serial.print(colorIndices[0]);
        Serial.print(colorIndices[1]);

        delete[] colorIndices;
        colorIndices = nullptr;
        
        // digitalWrite(RED_LED, HIGH);
        // digitalWrite(BLUE_LED, HIGH);
        // int *colors = colorSensed();
        // colorMedian();
        // x++;
        
        // IDEA: FUNCTIONS FOR LANE FOLLOWING AND FINDING?
        // switch(currState){
        //     case state1: /* cross to the other side */
        //         while (!objectDetected()) {
        //             motions.forward();
        //         }

        //         currState = state2; 
        //         break;
        //     case state2: /* stop and turn 180 degrees*/
        //         motions.stop();

        //         motions.right_turn(); // FIX DELAY
        //         delay(1000);
        //         motions.stop();

        //         currState = state3;
        //         break;
        //     case state3: /* move forward until reached red lane*/
        //         while (colorSensed() != RED_INDEX) {
        //             motions.backward();
        //         }

        //         currState = state4;
        //         break;
        //     case state4: /* follow red lane */
        //         motions.left_turn(); // FIX DELAY
        //         delay(1000);
        //         motions.stop();

        //         while (!objectDetected()) {
        //             while (colorSensed() == RED_INDEX) {
        //                 motions.forward();
        //             }
        //         }

        //         motions.stop();
        //         currState = state5;
        //         break;
        //     case state5: /* turn left and find yellow lane */
        //         motions.left_turn(); // FIX DELAY
        //         delay(1000);
        //         motions.stop();
                
        //         while (colorSensed() != YELLOW_INDEX) {
        //             motions.forward();
        //         }
        //         motions.stop();
        //         motions.left_turn(); // fIX DELAY
        //         delay(1000);
        //         motions.stop();

        //         currState = state6; // IDEA: TURN WHILE FINDING COLOR? LIKE TURN 1 DEGREE SEE COLOR TURN 1 DEGREE SEE COLOR?
        //         break;
        //     case state6:
        //         while (!objectDetected()) {
        //             while (colorSensed() == YELLOW_INDEX) {
        //                 motions.forward();
        //             }
        //         }
        //         motions.stop();
        //         currState = state7;
        //         break;
        //     case state7:
        //         motions.left_turn(); // FIX DELAY
        //         delay(1000);
        //         motions.stop();

        //         while (!objectDetected()) {
        //             motions.forward();
        //         }

        //         motions.stop();
        //         break;
        // }
    // }
}

