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

int calVals[4][2][2][2];


void sendMessage(String messageToTeam){
    client.beginMessage(TYPE_TEXT);
    client.print(messageToTeam);
    client.endMessage();
    Serial.println("sent");
    sentOnce = false;
}

void calibrateOnce(int color) {

    for (int i = 0; i < 20; i++) {
        // --- RED illumination ---
        digitalWrite(RED_LED, HIGH);
        delay(5);
        int r1 = analogRead(PHOTO_TRANS_1);
        int r2 = analogRead(PHOTO_TRANS_2);
        digitalWrite(RED_LED, LOW);


        // --- BLUE illumination ---
        digitalWrite(BLUE_LED, HIGH);
        delay(5);
        int b1 = analogRead(PHOTO_TRANS_1);
        int b2 = analogRead(PHOTO_TRANS_2);
        digitalWrite(BLUE_LED, LOW);


        // sensor 1
        calVals[color][SENSOR1][RED_CH][MIN_V] = min(calVals[color][SENSOR1][RED_CH][MIN_V], r1);
        calVals[color][SENSOR1][RED_CH][MAX_V] = max(calVals[color][SENSOR1][RED_CH][MAX_V], r1);
        calVals[color][SENSOR1][BLUE_CH][MIN_V] = min(calVals[color][SENSOR1][BLUE_CH][MIN_V], b1);
        calVals[color][SENSOR1][BLUE_CH][MAX_V] = max(calVals[color][SENSOR1][BLUE_CH][MAX_V], b1);


        // sensor 2
        calVals[color][SENSOR2][RED_CH][MIN_V] = min(calVals[color][SENSOR2][RED_CH][MIN_V], r2);
        calVals[color][SENSOR2][RED_CH][MAX_V] = max(calVals[color][SENSOR2][RED_CH][MAX_V], r2);
        calVals[color][SENSOR2][BLUE_CH][MIN_V] = min(calVals[color][SENSOR2][BLUE_CH][MIN_V], b2);
        calVals[color][SENSOR2][BLUE_CH][MAX_V] = max(calVals[color][SENSOR2][BLUE_CH][MAX_V], b2);

        delay(40);
    }
}

void calibrateAllColors() {
    for (int c = RED; c <= BLACK; c++) {
        waitForButtonPress();   // user positions object
        calibrateOnce(c);
        digitalWrite(LED_BUILTIN, HIGH);
        delay(150);
        digitalWrite(LED_BUILTIN, LOW);
    }
}


void waitForButtonPress() {
    while (digitalRead(BUTTON_PIN) == HIGH) {
        // do nothing, just wait
    }
    delay(20); // debounce
    while (digitalRead(BUTTON_PIN) == LOW); // wait for release
}

int colorIndexSingle(int redAvg, int blueAvg, int sensor) {

    if (
        (redAvg  >= calVals[RED][sensor][RED_CH][MIN_V]  &&
         redAvg  <= calVals[RED][sensor][RED_CH][MAX_V]) &&
        (blueAvg >= calVals[RED][sensor][BLUE_CH][MIN_V] &&
         blueAvg <= calVals[RED][sensor][BLUE_CH][MAX_V])
    ) {
        return RED;
    }

    else if (
        (redAvg  >= calVals[YELLOW][sensor][RED_CH][MIN_V]  &&
         redAvg  <= calVals[YELLOW][sensor][RED_CH][MAX_V]) &&
        (blueAvg >= calVals[YELLOW][sensor][BLUE_CH][MIN_V] &&
         blueAvg <= calVals[YELLOW][sensor][BLUE_CH][MAX_V])
    ) {
        return YELLOW;
    }

    else if (
        (redAvg  >= calVals[BLUE][sensor][RED_CH][MIN_V]  &&
         redAvg  <= calVals[BLUE][sensor][RED_CH][MAX_V]) &&
        (blueAvg >= calVals[BLUE][sensor][BLUE_CH][MIN_V] &&
         blueAvg <= calVals[BLUE][sensor][BLUE_CH][MAX_V])
    ) {
        return BLUE;
    }

    else if (
        (redAvg  >= calVals[BLACK][sensor][RED_CH][MIN_V]  &&
         redAvg  <= calVals[BLACK][sensor][RED_CH][MAX_V]) &&
        (blueAvg >= calVals[BLACK][sensor][BLUE_CH][MIN_V] &&
         blueAvg <= calVals[BLACK][sensor][BLUE_CH][MAX_V])
    ) {
        return BLACK;
    }

    return WRONG;
}






int *colorSensed() {
    redBuffer1.clear();
    blueBuffer1.clear();

    redBuffer2.clear();
    blueBuffer2.clear();

    // taking 10 phototransistor values for each color and each sensor, then 
    // and pushing them to the corresponding buffer
    for (int i = 0; i < 5; i++) {
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
    Serial.print(blueAverage2);
    Serial.print(": ");

    int *colorIndices = new int[2];

    // passing the red and blue average values for each sensor to a helper 
    // function to get what color it is 

    colorIndices[0] = colorIndexSingle(redAverage1, blueAverage1, SENSOR1);
    colorIndices[1] = colorIndexSingle(redAverage2, blueAverage2, SENSOR2);

    Serial.print(colorIndices[0]);
    Serial.println(colorIndices[1]);

    return colorIndices;
}

int colorIndex1(int redAverage, int blueAverage) { // just change everytime
    if ((redAverage >= 3 && redAverage <= 22) && (blueAverage >= 7 && blueAverage <= 81)) {
        return RED;
    } else if ((redAverage >= 8 && redAverage <= 23) && (blueAverage >= 101 && blueAverage <= 113)) {
        return BLUE;
    } else if ((redAverage >= 49 && redAverage <= 59) && (blueAverage >= 96 && blueAverage <= 101)) {
        return YELLOW;
    } else if ((redAverage >= 5 && redAverage <= 17) && (blueAverage >= 54 && blueAverage <= 65)) {
        return BLACK;
    } else {
        return WRONG;
    }
}

int colorIndex2(int redAverage, int blueAverage) {
    if ((redAverage >= 123 && redAverage <= 133) && (blueAverage >= 45 && blueAverage <= 49)) {
        return RED;
    } else if ((redAverage >= 99 && redAverage <= 110) && (blueAverage >= 130 && blueAverage <= 153)) {
        return BLUE;
    } else if ((redAverage >= 300 && redAverage <= 311) && (blueAverage >= 47 && blueAverage <= 50)) {
        return YELLOW;
    } else if ((redAverage >= 76 && redAverage <= 119) && (blueAverage >= 78 && blueAverage <= 98)) {
        return BLACK;
    } else {
        return WRONG;
    }
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
    float comparisonVal = -40; //change

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
    // Serial.println("in forward");
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