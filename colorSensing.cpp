#include "colorSensing.h"

int calVals[4][2][2][2];

void calibrateOnce(int color) {
    
    for (int i = 0; i < 10; i++) {
        // --- RED illumination ---
        digitalWrite(RED_LED, HIGH);
        float r1 = analogRead(PHOTO_TRANS_1);
        Serial.print("r1 is ");
        Serial.println(r1);

        float r2 = analogRead(PHOTO_TRANS_2);
        Serial.print("r2 is ");
        Serial.println(r2);
        delay(40);
        digitalWrite(RED_LED, LOW);
        

        // --- BLUE illumination ---
        digitalWrite(BLUE_LED, HIGH);
        float b1 = analogRead(PHOTO_TRANS_1);
        float b2 = analogRead(PHOTO_TRANS_2);
        digitalWrite(BLUE_LED, LOW);

// 89 107 108 131 min max min max
// 159 169 202 218
// 60 72 65 89
// 27 77 47 75
        // sensor 1
        calVals[color][SENSOR1][RED_CH][MIN_V] = min(calVals[color][SENSOR1][RED_CH][MIN_V], r1);
        Serial.print("RED SENSOR1 MIN: ");
        Serial.println(calVals[color][SENSOR1][RED_CH][MIN_V]);

        calVals[color][SENSOR1][RED_CH][MAX_V] = max(calVals[color][SENSOR1][RED_CH][MAX_V], r1);
        Serial.print("RED SENSOR1 MAX: ");
        Serial.println(calVals[color][SENSOR1][RED_CH][MAX_V]);


        calVals[color][SENSOR1][BLUE_CH][MIN_V] = min(calVals[color][SENSOR1][BLUE_CH][MIN_V], b1);
        calVals[color][SENSOR1][BLUE_CH][MAX_V] = max(calVals[color][SENSOR1][BLUE_CH][MAX_V], b1);


        // sensor 2
        calVals[color][SENSOR2][RED_CH][MIN_V] = min(calVals[color][SENSOR2][RED_CH][MIN_V], r2);
        Serial.print("RED SENSOR2 MIN: ");
        Serial.println(calVals[color][SENSOR2][RED_CH][MIN_V]);

        calVals[color][SENSOR2][RED_CH][MAX_V] = max(calVals[color][SENSOR2][RED_CH][MAX_V], r2);
        Serial.print("RED SENSOR2 max: ");
        Serial.println(calVals[color][SENSOR2][RED_CH][MAX_V]);

        calVals[color][SENSOR2][BLUE_CH][MIN_V] = min(calVals[color][SENSOR2][BLUE_CH][MIN_V], b2);
        calVals[color][SENSOR2][BLUE_CH][MAX_V] = max(calVals[color][SENSOR2][BLUE_CH][MAX_V], b2);

        delay(40);
    }
}

void initCalibration(int color) {
    for (int s = 0; s < 2; s++) {
        for (int ch = 0; ch < 2; ch++) {
            calVals[color][s][ch][MIN_V] = 1023;  // max possible ADC value
            calVals[color][s][ch][MAX_V] = 0;
        }
    }
}


void calibrateAllColors() {
    for (int c = RED; c <= BLACK; c++) {
        initCalibration(c);
        Serial.print("calibrating for ");
        Serial.println(c);
        waitForButtonPress();   // user positions object
        Serial.println("button pressed");
        calibrateOnce(c);
        applyCalibrationBuffer(c);

        digitalWrite(LED_BUILTIN, HIGH);
        delay(150);
        digitalWrite(LED_BUILTIN, LOW);
    }
    waitForButtonPress();
}


void waitForButtonPress() {
    while (digitalRead(BUTTON_PIN) == HIGH) { // HIGH means not pressed 
        // waiting for press
    }
    delay(20); // debounce
    return;    // exit immediately on press
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
// take the samples, put a bigger delay between flashes, more samples
void applyCalibrationBuffer(int color) {
    const int BUFFER = 5;

    for (int s = 0; s < 2; s++) {
        for (int ch = 0; ch < 2; ch++) {
            calVals[color][s][ch][MIN_V] = 
                max(0, calVals[color][s][ch][MIN_V] - BUFFER);

            calVals[color][s][ch][MAX_V] =
                min(1023, calVals[color][s][ch][MAX_V] + BUFFER);
        }
    }
}


int *colorSensed() {
    redBuffer1.clear();
    blueBuffer1.clear();

    redBuffer2.clear();
    blueBuffer2.clear();

    // taking 10 phototransistor values for each color and each sensor, then 
    // and pushing them to the corresponding buffer
    for (int i = 0; i < 1; i++) {
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