#include "functions.h"

String receiveMessage() {
    int messageReceived = client.parseMessage();
    while (!messageReceived) {    
        messageReceived = client.parseMessage();
    }

    String parsed;
    if (messageReceived) {
        String message = client.readString();
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

int colorSensed() {


    //  /* check red */
    // // turn red LED on, yellow off
    // digitalWrite(RED_LED, HIGH);
    // // read from sensor
    // float vout1 = analogRead(PHOTO_TRANS);
    // Serial.print("red led is ");
    // Serial.println(vout1);
    // delay(40);

    // /* check blue */
    // // turn red off, blue on 
    // digitalWrite(RED_LED , LOW);
    // digitalWrite(BLUE_LED , HIGH);

    // // read from sensor
    // float vout2 = analogRead(PHOTO_TRANS);
    // Serial.print("blue led is: ");
    // Serial.println(vout2);
    // delay(40);

    // digitalWrite(BLUE_LED, LOW);

    redBuffer.clear();
    blueBuffer.clear();

    for (int i = 0; i < 10; i++) {
        digitalWrite(RED_LED, HIGH);
        float red = analogRead(PHOTO_TRANS);
        redBuffer.push(red);
        delay(40);

        // Serial.print(red);
        // Serial.print(", ");

        digitalWrite(RED_LED , LOW);
        digitalWrite(BLUE_LED , HIGH);

        float blue = analogRead(PHOTO_TRANS);
        blueBuffer.push(blue);

        digitalWrite(BLUE_LED , LOW);
        // Serial.print(blue);
        // Serial.println(", ");
        delay(40);
    }

    float redSum = 0;
    float blueSum = 0;

    for (int i = 0; i < redBuffer.size(); i++) {
        redSum += redBuffer[i];
        blueSum += blueBuffer[i];
    }

    float redAverage = redSum / redBuffer.size();
    float blueAverage = blueSum / blueBuffer.size();


    Serial.print(redAverage);
    Serial.print(",");

    Serial.print(blueAverage);
    Serial.println("");

    if ((redAverage > 11 && redAverage < 16) && (blueAverage > 23 && blueAverage < 29)) {
        return RED_INDEX;
    } else if ((redAverage > 6 && redAverage < 11) && (blueAverage > 17 && blueAverage < 19)) {
        return BLUE_INDEX;
    } else if ((redAverage > 40) && (blueAverage > 52)) {
        return YELLOW_INDEX;
    } else if ((redAverage > 0 && redAverage < 2) && (blueAverage > 4 && blueAverage < 9)) {
        return BLACK_INDEX;
    } else {
        return WRONG_INDEX;
    }

    
/*
    	red	blue
red	    11-16	23-29
yellow	40+	52+
blue	6-11	17-19
black	0-2	4-9

*/
  
}

// float calculateMedian(int arr[], int size) {
//   // Sort the array (using bubble sort for simplicity)
//   for (int i = 0; i < size - 1; i++) {
//     for (int j = 0; j < size - i - 1; j++) {
//       if (arr[j] > arr[j + 1]) {
//         int temp = arr[j];
//         arr[j] = arr[j + 1];
//         arr[j + 1] = temp;
//       }
//     }
//   }

//   // Calculate the median
//   if (size % 2 == 0) { // Even number of elements
//     return (float)(arr[size / 2 - 1] + arr[size / 2]) / 2.0;
//   } else { // Odd number of elements
//     return (float)arr[size / 2];
//   }
// }


// int colorMedian() {
//     int *colors = colorSensed();
//     int ambientBuffer[50];
//     int redBuffer[50];
//     int blueBuffer[50];
//     int currentIndex = 0;

//     for (int i = 0; i < 50; i++) {
//         if (currentIndex < 50) {
//             ambientBuffer[currentIndex] = colors[0];
//             redBuffer[currentIndex] = colors[1];
//             blueBuffer[currentIndex] = colors[2];
//             currentIndex++;
//         }
//     }

//     int ambientMedian = calculateMedian(ambientBuffer, 50);
//     int redMedian = calculateMedian(redBuffer, 50);
//     int blueMedian = calculateMedian(blueBuffer, 50);

//     int redIsolated = redMedian - ambientMedian; // use mean/median
//     int blueIsolated = blueMedian - ambientMedian;

//     Serial.print(redIsolated);
//     Serial.print(",");
//     Serial.println(blueIsolated);

// }

// bool objectDetected() {

//     if (!initialDetection) {

//         digitalWrite(IR_LED, HIGH);

//         photoBuffer.clear();

//         for (int i = 0; i < photoBuffer.capacity; i++) {
//             int reading = analogRead(PHOTO_DETECTOR);

//             photoBuffer.push(reading);
//             delay(2);
//         }

//         //baseline
//         long sum = 0;
//         for (size_t i = 0; i < photoBuffer.size(); i++) {
//             sum += photoBuffer[i];
//         }
//         initialReading = sum / photoBuffer.size();

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

//     if ((average - initialReading) > 30) {
//         return true;
//     }

//     return false;
// }


void BotMotions::stop() {
    // Turn off motors
    digitalWrite(in1, LOW);
    digitalWrite(in2, LOW);
    digitalWrite(in3, LOW);
    digitalWrite(in4, LOW);
}

void BotMotions::backward() {
    digitalWrite(enA, HIGH);
    digitalWrite(enB, HIGH);
    // Turn on motors
    digitalWrite(in1, LOW); //right motor
    digitalWrite(in3, LOW); //left motor
    digitalWrite(in2, HIGH); //right motor
    digitalWrite(in4, HIGH); //left motor

    analogWrite(enB, max_speed); //left motor enable
    analogWrite(enA, max_speed); //right motor enable
    delay(20);
}

void BotMotions::forward() {
    Serial.println("in forward");
    digitalWrite(enA, HIGH);
    digitalWrite(enB, HIGH);
    // Turn on motors
    digitalWrite(in1, HIGH); //right motor
    digitalWrite(in3, HIGH); //left motor
    digitalWrite(in2, LOW); //right motor
    digitalWrite(in4, LOW); //left motor

    analogWrite(enB, max_speed);
    analogWrite(enA, max_speed);
    delay(20);
}

void BotMotions::pivot_c() {
    digitalWrite(enA, HIGH);
    digitalWrite(enB, HIGH);
    // Turn on motors
    digitalWrite(in1, LOW); //right motor
    digitalWrite(in3, HIGH); //left motor
    digitalWrite(in2, LOW); //right motor
    digitalWrite(in4, LOW); //left motor

    analogWrite(enB, max_speed);
    analogWrite(enA, max_speed);
    delay(20);

}

void BotMotions::pivot_cc() {
    digitalWrite(enA, HIGH);
    digitalWrite(enB, HIGH);
    // Turn on motors
    digitalWrite(in1, HIGH); //right motor
    digitalWrite(in3, LOW); //left motor
    digitalWrite(in2, LOW); //right motor
    digitalWrite(in4, LOW); //left motor

    analogWrite(enB, max_speed);
    analogWrite(enA, max_speed);
    delay(20);

}

void BotMotions::right_turn() {
    //LEFT SPEED HAS TO BE SET HIGHER

    digitalWrite(enA, HIGH);
    digitalWrite(enB, HIGH);
    // Turn on motors
    digitalWrite(in1, HIGH); //right motor
    digitalWrite(in3, HIGH); //left motor
    digitalWrite(in2, LOW); //right motor
    digitalWrite(in4, LOW); //left motor


    analogWrite(enB, left_speed);
    analogWrite(enA, right_speed);
    delay(20);

}

void BotMotions::left_turn() {
    //RIGHT SPEED HAS TO BE HIGHER
    //Note: left motor is weak, so we need to have at least 50

    digitalWrite(enA, HIGH);
    digitalWrite(enB, HIGH);
    // Turn on motors
    digitalWrite(in1, HIGH); //right motor
    digitalWrite(in3, HIGH); //left motor
    digitalWrite(in2, LOW); //right motor
    digitalWrite(in4, LOW); //left motor


    analogWrite(enB, left_speed);
    analogWrite(enA, right_speed);
    delay(20);

}

void BotMotions::set_speeds(int left_speed, int right_speed, int max_speed) {
    this->left_speed = left_speed;
    this->right_speed = right_speed;
    this->max_speed = max_speed;
}

void setPinModes() {
  /* Color Detection and Object Detection I/O */
  pinMode(RED_LED , OUTPUT);
  pinMode(BLUE_LED, OUTPUT);
  pinMode(IR_LED, OUTPUT);
  pinMode(PHOTO_TRANS, INPUT);
  pinMode(PHOTO_DETECTOR , INPUT);

  /* Motor Control I/O */
  pinMode(enA, OUTPUT);
  pinMode(enB, OUTPUT);
  pinMode(in1, OUTPUT);
  pinMode(in2, OUTPUT);
  pinMode(in3, OUTPUT);
  pinMode(in4, OUTPUT);

  /* Turn off motors - Initial state */ 
  digitalWrite(in1, LOW);
  digitalWrite(in2, LOW);
  digitalWrite(in3, LOW);
  digitalWrite(in4, LOW);
  digitalWrite(enA, LOW);
  digitalWrite(enB, LOW);
}

void wifiSetup() {
  while (status != WL_CONNECTED) {
    Serial.print("Attempting to connect to Network named: ");
    Serial.println(ssid); // print the network name (SSID);

    // Connect to WPA/WPA2 network:
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
//1000 for red, 700 for blue