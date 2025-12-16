#ifndef COLORSENSING_H
#define COLORSENSING_H

#include "def.h"

int *colorSensed();
int colorIndex1(int redAverage, int blueAverage);
int colorIndex2(int redAverage, int blueAverage);
void waitForButtonPress();
void calibrateOnce(int color);
void calibrateAllColors();
int colorIndexSingle(int redAvg, int blueAvg, int sensor);
void initCalibration(int color);
void applyCalibrationBuffer(int color);

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