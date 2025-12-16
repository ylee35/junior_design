
/*
 *      functions.h
 * 
 *      This file contains function declarations for all helper functions used. 
 *      It also defines the BotMotions class. 
 * 
*/

#ifndef FUNCTIONS_H
#define FUNCTIONS_H

#include "def.h"

int *colorSensed();
int colorIndex1(int redAverage, int blueAverage);
int colorIndex2(int redAverage, int blueAverage);
bool objectDetected();
void setPinModes();
void wifiSetup();
void sendMessage(String messageToTeam);
String receiveMessage();

void waitForButtonPress();
void calibrateOnce(int color);
void calibrateAllColors();
int colorIndexSingle(int redAvg, int blueAvg, int sensor);
void initCalibration(int color);
void applyCalibrationBuffer(int color);


class BotMotions {
    public: 
        void directionControl();
        void speedControl();
        void forward();
        void stop();
        void backward();
        void pivot_c();
        void pivot_cc();
        void right_turn();
        void left_turn();
        void set_speeds(int left_speed, int right_speed, int max_speed);
    
    private: 
        int left_speed;
        int right_speed;
        int max_speed;
};

struct ColorCal {
    int red1Min, red1Max;
    int blue1Min, blue1Max;
    int red2Min, red2Max;
    int blue2Min, blue2Max;
};

#define SENSOR1 0
#define SENSOR2 1
#define RED_CH  0
#define BLUE_CH 1
#define MIN_V   0
#define MAX_V   1



#endif