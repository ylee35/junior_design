
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



// int array[4] = {0, 0, 0, 0};
bool firstTime = true; 

// bool objectDetected() {
//     digitalWrite(IR_LED, HIGH);
//     if (firstTime) {
//         for (int i = 0; i < 4; i++) {

//             float reading = analogRead(PHOTO_DETECTOR);

//             array[i] = reading;
//         }
//         firstTime = false;
//     } else {

//         float reading = analogRead(PHOTO_DETECTOR);

//         array[0] = array[1];
//         array[1] = array[2];
//         array[2] = array[3];
//         array[3] = reading;

//         // Serial.print("difference is ");
//         // Serial.println(array[3] - array[0]);

//         if (array[3] - array[0] < -60) {
//             Serial.println("---------RETURNING TRUE---------");
//             return true;
//         }

//         return false;
//     }

// }

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
    float comparisonVal = -125; //change

    float difference = baseline - newReading;
    Serial.print("difference is ");
    Serial.println(difference);
    //if (color is yel)
    /*else*/if (difference < comparisonVal) {
        return true;
    }

    return false;
}

// bool objectDetected() {
    
//     if (!initialDetection) {

//         digitalWrite(IR_LED, HIGH);

//         photoBuffer.clear();

//         // for (int i = 0; i < photoBuffer.capacity; i++) {
//         //     int reading = analogRead(PHOTO_DETECTOR);

//         //     photoBuffer.push(reading);
//         //     delay(2);
//         // }

//         // //baseline
//         // long sum = 0;
//         // for (size_t i = 0; i < photoBuffer.size(); i++) {
//         //     sum += photoBuffer[i];
//         // }
//         // initialReading = sum / photoBuffer.size();


//         delay(100);
//         initialReading = analogRead(PHOTO_DETECTOR);

//         Serial.print("Baseline: ");
//         Serial.println(initialReading);

//         initialDetection = true;
//     }
//     digitalWrite(IR_LED, HIGH);

//     float reading = analogRead(PHOTO_DETECTOR);

//     photoBuffer.push(reading);

//     float sum = 0;
//     for (size_t i = 0; i < photoBuffer.size(); i++) {
//         sum += photoBuffer[i];
//     }
//     float average = sum / photoBuffer.size();

//     Serial.print("Raw photodetector: ");
//     Serial.print(reading);
//     Serial.print(" | Filtered average: ");
//     Serial.println(average);

//     Serial.print("******** DIFFERENCE IS ");
//     Serial.println(average - initialReading);

//     if ((average - initialReading) > 12) { // higher if closer
        
//         Serial.println("---------RETURNING TRUE---------");
//         return true;
//     }

//     return false;
// }




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