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
    Serial.println("sent");
    sentOnce = false;
}

int *colorSensed() {
    redBuffer1.clear();
    blueBuffer1.clear();

    redBuffer2.clear();
    blueBuffer2.clear();

    // taking 10 phototransistor values for each color and each sensor, then 
    // and pushing them to the corresponding buffer
    for (int i = 0; i < 10; i++) {
        digitalWrite(RED_LED, HIGH);
        float red1 = analogRead(PHOTO_TRANS_1);
        float red2 = analogRead(PHOTO_TRANS_2);
        redBuffer1.push(red1);
        redBuffer2.push(red2);

        delay(40);

        // Serial.print("red raw: ");
        // Serial.print(red1);
        // Serial.print(", ");

        digitalWrite(RED_LED , LOW);
        digitalWrite(BLUE_LED , HIGH);

        float blue1 = analogRead(PHOTO_TRANS_1);
        float blue2 = analogRead(PHOTO_TRANS_2);
        blueBuffer1.push(blue1);
        blueBuffer2.push(blue2);

        digitalWrite(BLUE_LED , LOW);
        // Serial.print(blue);
        // Serial.println(", ");
        delay(40);
    }

    float redSum1 = 0;
    float redSum2 = 0;
    float blueSum1 = 0;
    float blueSum2 = 0;
    
    // finding the average of the 10 values
    for (int i = 0; i < redBuffer1.size(); i++) {
        redSum1 += redBuffer1[i];
        redSum2 += redBuffer2[i];
        blueSum1 += blueBuffer1[i];
        blueSum2 += blueBuffer2[i];
    }

    float redAverage1 = redSum1 / redBuffer1.size();
    float redAverage2 = redSum2 / redBuffer2.size();
    float blueAverage1 = blueSum1 / blueBuffer1.size();
    float blueAverage2 = blueSum2 / blueBuffer2.size();

    // REMOVE
    Serial.print(redAverage1);
    Serial.print(",");
    Serial.print(redAverage2);
    Serial.print(",");

    Serial.print(blueAverage1);
    Serial.print(",");
    Serial.println(blueAverage2);
    // Serial.print(":      ");

    int *colorIndices = new int[2];

    // passing the red and blue average values for each sensor to a helper 
    // function to get what color it is 
    colorIndices[0] = colorIndex1(redAverage1, blueAverage1);
    colorIndices[1] = colorIndex2(redAverage2, blueAverage2);

    return colorIndices;
}

int colorIndex1(int redAverage, int blueAverage) { // just change everytime
    if ((redAverage >= 296 && redAverage <= 305) && (blueAverage >= 359 && blueAverage <= 365)) {
        return RED;
    } else if ((redAverage >= 260 && redAverage <= 274) && (blueAverage >= 345 && blueAverage <= 353)) {
        return BLUE;
    } else if ((redAverage >= 389) && (blueAverage >= 438)) {
        return YELLOW;
    } else if ((redAverage >= 237 && redAverage <= 253) && (blueAverage >= 291 && blueAverage <= 302)) {
        return BLACK;
    } else {
        return WRONG;
    }
}

int colorIndex2(int redAverage, int blueAverage) {
    if ((redAverage >= 41 && redAverage <= 44) && (blueAverage >= 28 && blueAverage <= 31)) {
        return RED;
    } else if ((redAverage >= 30 && redAverage <= 33) && (blueAverage >= 52 && blueAverage <= 55)) {
        return BLUE;
    } else if ((redAverage >= 82) && (blueAverage >= 44)) {
        return YELLOW;
    } else if ((redAverage >= 23 && redAverage <= 26) && (blueAverage >= 22 && blueAverage <= 24)) {
        return BLACK;
    } else {
        return WRONG;
    }
}

bool objectDetected() {

    if (!initialDetection) {

        digitalWrite(IR_LED, HIGH);

        photoBuffer.clear();

        for (int i = 0; i < photoBuffer.capacity; i++) {
            int reading = analogRead(PHOTO_DETECTOR);

            photoBuffer.push(reading);
            delay(2);
        }

        //baseline
        long sum = 0;
        for (size_t i = 0; i < photoBuffer.size(); i++) {
            sum += photoBuffer[i];
        }
        initialReading = sum / photoBuffer.size();

        Serial.print("Baseline: ");
        Serial.println(initialReading);

        initialDetection = true;
    }
    digitalWrite(IR_LED, HIGH);

    float reading = analogRead(PHOTO_DETECTOR);

    photoBuffer.push(reading);

    float sum = 0;
    for (size_t i = 0; i < photoBuffer.size(); i++) {
        sum += photoBuffer[i];
    }
    float average = sum / photoBuffer.size();

    Serial.print("Raw photodetector: ");
    Serial.print(reading);
    Serial.print(" | Filtered average: ");
    Serial.println(average);

    if ((average - initialReading) > 30) {
        return true;
    }

    return false;
}


void BotMotions::stop() {
    // turn off motors
    digitalWrite(in1, LOW);
    digitalWrite(in2, LOW);
    digitalWrite(in3, LOW);
    digitalWrite(in4, LOW);
}

void BotMotions::backward() {
    digitalWrite(enA, HIGH);
    digitalWrite(enB, HIGH);

    // turn on motors
    digitalWrite(in1, LOW); // right motor
    digitalWrite(in3, LOW); // left motor
    digitalWrite(in2, HIGH); // right motor
    digitalWrite(in4, HIGH); // left motor

    analogWrite(enB, max_speed); // left motor enable
    analogWrite(enA, max_speed); // right motor enable
    delay(20);
}

void BotMotions::forward() {
    Serial.println("in forward");
    digitalWrite(enA, HIGH);
    digitalWrite(enB, HIGH);

    // turn on motors
    digitalWrite(in1, HIGH); // right motor
    digitalWrite(in3, HIGH); // left motor
    digitalWrite(in2, LOW); // right motor
    digitalWrite(in4, LOW); // left motor

    analogWrite(enB, max_speed);
    analogWrite(enA, max_speed);
    delay(20);
}

void BotMotions::pivot_c() {
    digitalWrite(enA, HIGH);
    digitalWrite(enB, HIGH);

    // turn on motors
    digitalWrite(in1, LOW); // right motor
    digitalWrite(in3, HIGH); // left motor
    digitalWrite(in2, LOW); // right motor
    digitalWrite(in4, LOW); // left motor

    analogWrite(enB, max_speed);
    analogWrite(enA, max_speed);
    delay(20);
}

void BotMotions::pivot_cc() {
    digitalWrite(enA, HIGH);
    digitalWrite(enB, HIGH);

    // turn on motors
    digitalWrite(in1, HIGH); // right motor
    digitalWrite(in3, LOW); // left motor
    digitalWrite(in2, LOW); // right motor
    digitalWrite(in4, LOW); // left motor

    analogWrite(enB, max_speed);
    analogWrite(enA, max_speed);
    delay(20);
}

void BotMotions::right_turn() {
    // LEFT SPEED HAS TO BE HIGHER 
    digitalWrite(enA, HIGH);
    digitalWrite(enB, HIGH);

    // turn on motors
    digitalWrite(in1, HIGH); // right motor
    digitalWrite(in3, HIGH); // left motor
    digitalWrite(in2, LOW); // right motor
    digitalWrite(in4, LOW); // left motor

    analogWrite(enB, left_speed);
    analogWrite(enA, right_speed);
    delay(20);
}

void BotMotions::left_turn() {
    //RIGHT SPEED HAS TO BE HIGHER
    //Note: left motor is weak, so we need to have at least 50

    digitalWrite(enA, HIGH);
    digitalWrite(enB, HIGH);

    // turn on motors
    digitalWrite(in1, HIGH); // right motor
    digitalWrite(in3, HIGH); // left motor
    digitalWrite(in2, LOW); // right motor
    digitalWrite(in4, LOW); // left motor

    analogWrite(enB, left_speed);
    analogWrite(enA, right_speed);
    delay(20);
}

void BotMotions::set_speeds(int left_speed, int right_speed, int max_speed) {
    this->left_speed = left_speed;
    this->right_speed = right_speed;
    this->max_speed = max_speed;
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
