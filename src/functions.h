#ifndef FUNCTIONS_H
#define FUNCTIONS_H

#include "def.h"

int colorSensed();
bool objectDetected();
void setPinModes();
void wifiSetup();
void sendMessage(String messageToTeam);
String receiveMessage();

float calculateMedian(int arr[], int size);
int colorMedian();

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

#endif
