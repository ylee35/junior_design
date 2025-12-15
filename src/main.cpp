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
    // client.begin();

    // Serial.println("starting WebSocket client");
    // client.begin();
    // sendMessage(clientID);

    // client.beginMessage(TYPE_TEXT);
    // client.print(clientID);
    // client.endMessage();

    // while (client.connected()) {

    motions.set_speeds(100, 100, 100);
    // Serial.print(x);
    // Serial.print(",");
    // motions.stop();
    int* colorIndices = colorSensed();

    delete[] colorIndices;
    colorIndices = nullptr;

    // motions.forward();
    // bool object = objectDetected();
    // Serial.println(object);
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

    while(1){
        switch(currState) {
            case state1: // cross to the other side
                // Serial.println("in state1");
                // motions.forward();
                while(count < 20){
                    object = objectDetected();
                    count++;
                    motions.forward();
                }

                object = objectDetected();

                if (object) {
                    motions.stop();
                    // digitalWrite(LED_BUILTIN, HIGH);
                    Serial.println("Object detected");
                    currState = state2;
                    Serial.println("Sending to state 2");
                    break;
                } else {
                    motions.forward();
                    digitalWrite(LED_BUILTIN, LOW);
                }
                currState = state1;
                // Serial.println("Sending to state 1");
                break;

        case state2: //stop state
            Serial.println("in state 2");
            motions.stop();
            delay(2000);

            // currState = state3;
            Serial.println("Sending to state 3)");
            break;

        case state3: //find red and pivot
            Serial.println("in state 3");
            digitalWrite(LED_BUILTIN, HIGH);
            // object = objectDetected();
            // while (!object) {
            // object = objectDetected();
            int *colors = colorSensed();

            // go backwards to find red lane
            while (colors[RIGHT] != RED && colors[LEFT] != RED) {
                motions.backward();
                colors = colorSensed();
                // delay(2000);
            }

            motions.stop();

            // turn roughly 90 degrees
            int turnTime = 0;
            while (turnTime < 220) {
                motions.pivot_c();
                turnTime++;
            }
            motions.stop();

            // }
            currState = state4;
            Serial.println("Moving to state four");
            break;

        case state4:
        motions.stop();
        // reposition to make both sensors on red
        // int *colors = colorSensed();
        // while (colors[RIGHT] != RED && colors[LEFT] != RED) {
        // motions.right_turn();
        // }
        // motions.stop();
        // object = objectDetected();

        // while (!object) {
        // colors = colorSensed();
        // while (colors[RIGHT] == RED && colors[LEFT] == RED) {
        // motions.forward();
        // colors = colorSensed();
        // }

        // while (colors[RIGHT] == BLACK && colors[LEFT] == RED) {
        // motions.left_turn();
        // colors = colorSensed();
        // }

        // while (colors[RIGHT] == RED && colors[LEFT] == BLACK) {
        // motions.right_turn();
        // colors = colorSensed();
        // }
        // }

        // motions.stop();
        // currState = state5;
        // break;
        // //object polling
        // // while wall not sensed
        // // while left and right are red
        // // go forward
        // //when left is red and right is black turn to left
        // //when right is red left is black turn to right
        // // while wall is sensed, stop and move to state 5 and break
        // //move to state 5

        // case state5: // turning to find the yellow lane
        // int backTime = 0;
        // while (backTime < 10) { // CALIBRATE LATER
        // motions.backward();
        // }
        // motions.stop();

        // colors = colorSensed();
        // while (colors[RIGHT] != BLACK || colors[LEFT] != BLACK) {
        // motions.left_turn();
        // colors = colorSensed();
        // }
        // motions.stop();

        // colors = colorSensed();
        // while (colors[RIGHT] != YELLOW || colors[LEFT] != YELLOW) {
        // motions.forward();
        // }

        // motions.stop();
        // currState = state6;
        // break;
        // //go backward slightly to have room to turn
        // //turn to left until both sense black
        // //go forward until both sense yellow
        // //go forward until both sense black
        // //turn left until both sense yellow
        // //turn left until left senses black
        // //turn right until both sense yellow
        // //go to state 6, break

        // case state6: // moving in the yellow lane
        // // reposition to make both sensors on yellow
        // colors = colorSensed();
        // while (colors[RIGHT] != YELLOW && colors[LEFT] != YELLOW) {
        // motions.left_turn();
        // }
        // motions.stop();
        // object = objectDetected();

        // while (!object) {
        // int *colors = colorSensed();
        // while (colors[RIGHT] == YELLOW && colors[LEFT] == YELLOW) {
        // motions.forward();
        // colors = colorSensed();
        // }

        // while (colors[RIGHT] == BLACK && colors[LEFT] == YELLOW) {
        // motions.left_turn();
        // colors = colorSensed();
        // }

        // while (colors[RIGHT] == RED && colors[LEFT] == YELLOW) {
        // motions.right_turn();
        // colors = colorSensed();
        // }
        // }

        // motions.stop();
        // currState = state7;
        // break;
        // // bool object = objectDetected();
        // // while(object){
        // // motions.stop();
        // // bool object = objectDetected();
        // // Serial.println(object);
        // // currState = state7;
        // // break;
        // // }
        // // while(!object){
        // // motions.forward();
        // // bool object = objectDetected();
        // // Serial.println(object);
        // // }
        // // currState = state7;

        // default: // go back to where we started
        // int turnTime = 0;
        // while (turnTime < 10) { // CHANGE THIS VALUE
        // motions.left_turn();
        // turnTime++;
        // }

        // object = objectDetected();
        // while (!object) {
        // motions.forward();
        // object = objectDetected();
        // }
        // motions.stop();
        // //turn right until both sense black
        // //go forward until wall detected
        // break;
        }
    }
}
