
/*
 *      functions.cpp
 * 
 *      This file contains function definitions for all helper functions used. 
 * 
*/

#include "functions.h"

int initialReading;
bool initialDetection;

void setPinModes() {
    // Color Detection and Object Detection I/O 
    pinMode(RED_LED , OUTPUT);
    pinMode(BLUE_LED, OUTPUT);
    pinMode(IR_LED, OUTPUT);
    pinMode(PHOTO_TRANS_1, INPUT);
    pinMode(PHOTO_TRANS_2 , INPUT);

    // Motor Control I/O 
    pinMode(enA, OUTPUT);
    pinMode(enB, OUTPUT);
    pinMode(in1, OUTPUT);
    pinMode(in2, OUTPUT);
    pinMode(in3, OUTPUT);
    pinMode(in4, OUTPUT);

    // Turn off motors - Initial state 
    digitalWrite(in1, LOW);
    digitalWrite(in2, LOW);
    digitalWrite(in3, LOW);
    digitalWrite(in4, LOW);
    digitalWrite(enA, LOW);
    digitalWrite(enB, LOW);

    pinMode(BUTTON_PIN, INPUT_PULLUP);
}

String receiveMessage() {
    int messageReceived = client.parseMessage();

    while (!messageReceived) {    
        messageReceived = client.parseMessage();
    }

    String parsed;
    if (messageReceived) {
        String message = client.readString();

        // parsing through the message to find our ID 
        for (int i = 0 ; i <= (messageReceived - 12); i++){
        if (message.substring(i , i + 12) == "828BD9E1B7C7"){ 
            for (int j = i + 12; j <= messageReceived; j++){
                if (message.substring(j , j + 1) == "."){
                    parsed = message.substring(j + 1 , messageReceived);
                    break;
                }
            }
            break;
        }
        }
    } 

    return parsed;
}



void sendMessage(String messageToTeam){
    client.beginMessage(TYPE_TEXT);
    client.print(messageToTeam);
    client.endMessage();
    sentOnce = false;
}



bool firstTime = true; 
float baseline; 

bool objectDetected(){
    digitalWrite(IR_LED, HIGH);
    
    if (firstTime){
        int count = 0;
        if (count < 25) {
            analogRead(PHOTO_DETECTOR);
            count++;
        }
        baseline = analogRead(PHOTO_DETECTOR);
        firstTime = false;
    }
    float newReading = analogRead(PHOTO_DETECTOR);
    float comparisonVal = -85; //change

    float difference = baseline - newReading;
    Serial.print("difference is ");
    Serial.println(difference);
    //if (color is yel)
    /*else*/if (difference < comparisonVal) {
        return true;
    }

    return false;
}


// Example lane states for ONE-sensor follower (use RIGHT sensor only)
enum LaneState { ON_COLOR, SEARCH_LEFT_1, SEARCH_RIGHT_1, BACKUP };
LaneState laneState = ON_COLOR;

void laneFollowing(int targetColor) {
    Serial.println("in lane following");
    BotMotions motions;
    motions.set_speeds(150,150,150);

    const int STEP_MS   = 150;   // small action step
    const int FWD_MS    = 150;
    const int BACK_MS   = 150;  // how long to back up when lost
    // ideas: drift or when you sense color, dont just go forward, go slightly left (blue second half)
    while (true) {
        if (objectDetected()) {
            motions.stop();
            return;
        }

        int *colors = colorSensed();
        bool onTarget = (colors[RIGHT] == targetColor);

        switch (laneState) {

            case ON_COLOR: {
                if (onTarget) {
                    // pulse forward, then re-check next loop
                    Serial.println("on target");
                    motions.forwardLeft();
                    delay(FWD_MS);
                    motions.stop();
                    laneState = ON_COLOR;
                } else {
                    motions.stop();
                    laneState = SEARCH_LEFT_1;
                }
                break;
            }

            case SEARCH_LEFT_1: {
                Serial.println("search left 1");
                if (onTarget) { laneState = ON_COLOR; break; }

                motions.pivot_cc();        // pivot left (CCW)
                delay(STEP_MS);
                motions.stop();

                // next loop will re-sense
                laneState = SEARCH_RIGHT_1;
                break;
            }

            // case SEARCH_LEFT_2: {
            //     Serial.println("search left 2");
            //     if (onTarget) { laneState = ON_COLOR; break; }

            //     motions.pivot_cc();        // pivot left more
            //     delay(STEP_MS);
            //     motions.stop();

            //     laneState = SEARCH_RIGHT_1;
            //     break;
            // }

            case SEARCH_RIGHT_1: {
                Serial.println("search right 1");
                if (onTarget) { laneState = ON_COLOR; break; }

                motions.pivot_c();         // pivot right (CW)
                delay(STEP_MS);
                motions.stop();

                motions.pivot_c();
                delay(STEP_MS);
                motions.stop();

                laneState = BACKUP;
                break;
            }

            // case SEARCH_RIGHT_2: {
            //     Serial.println("search right 2");
            //     if (onTarget) { laneState = ON_COLOR; break; }

            //     motions.pivot_c();         // pivot right more
            //     delay(STEP_MS);
            //     motions.stop();

            //     // If still not found by next loop, back up
            //     laneState = BACKUP;
            //     break;
            // }

            case BACKUP: {
                Serial.println("backup");
                // one more check before backing up
                if (onTarget) { laneState = ON_COLOR; break; }

                motions.pivot_cc(); 
                delay(STEP_MS);
                motions.stop();

                motions.backward();
                delay(BACK_MS);
                motions.stop();

                // restart search
                laneState = SEARCH_LEFT_1;
                break;
            }
        } // switch
    } // while
}



void wifiSetup() {
    while (status != WL_CONNECTED) {
        Serial.print("Attempting to connect to Network named: ");
        Serial.println(ssid); // print the network name (SSID);

        // connect to WPA/WPA2 network:
        status = WiFi.begin(ssid, pass);
    }

    // print the SSID of the network you're attached to:
    Serial.print("SSID: ");
    Serial.println(WiFi.SSID());

    // print your WiFi shield's IP address:
    IPAddress ip = WiFi.localIP();
    Serial.print("IP Address: ");
    Serial.println(ip);
}