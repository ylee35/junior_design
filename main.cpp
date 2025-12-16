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

    calibrateAllColors();
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
            motions.set_speeds(150, 150, 150);
            case state1: // cross to the other side
                // int sent = false;
                // if (!sent){
                //     sendMessage("State 1 ^-^");
                //     sent = true;
                // }
                Serial.println("in state1");
                //motions.forward();
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
            sendMessage("State 2 ^-^");
            Serial.println("in state 2");
            motions.stop();
            delay(500);

            // currState = state3;
            Serial.println("Sending to state 3)");
            currState = state3;
            break;

        case state3: //find red and pivot
            sendMessage("State 3 ^-^");
            Serial.println("in state 3");
            digitalWrite(LED_BUILTIN, HIGH);
            int *colors = colorSensed();

            // go backwards to find red lane
            motions.set_speeds(100, 100, 100);
            while (colors[RIGHT] != RED) {
                motions.backward();
                colors = colorSensed();
                // delay(2000);
            }
            motions.stop();
            delay(500);

            while ((colors[RIGHT] == RED) || (colors[LEFT] == BLACK)){
                motions.pivot_cc();
                colors = colorSensed();
            }
            //pivot until we sense black with the right and red with the left

            motions.stop();
            delay(5);

            //get to red on the right
            while (colors[RIGHT == BLACK]){
                motions.pivot_c();
                colors = colorSensed();
            }

            currState = state4;
            Serial.println("Moving to state four");
            break;

        case state4: //red lane following
            sendMessage("State 4 ^-^");
            Serial.println("in state 4");
            motions.stop();
            object = objectDetected();
            
            while (!object && colors[RIGHT] == RED) {
                Serial.println("right is red");
                motions.forward();
                colors = colorSensed();
                object = objectDetected();
            }
            while (!object && colors[RIGHT] != RED){
                Serial.println("right is not red");
                motions.pivot_c();
                colors = colorSensed();
                object = objectDetected();
            }
            if (object){
                Serial.println("stopping");
                currState = state5;
                motions.stop();
                break;
            } 
            currState = state4;
            break;
            motions.stop();

        case state5: // turning to find the yellow lane
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