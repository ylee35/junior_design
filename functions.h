

/*
 *      functions.h
 * 
 *      This file contains function declarations for all helper functions used. 
 *      It also defines the BotMotions class. 
 * 
*/

#ifndef FUNCTIONS_H
#define FUNCTIONS_H

#include "motions.h"
#include "colorSensing.h"

bool objectDetected();
void setPinModes();
void wifiSetup();
void sendMessage(String messageToTeam);
String receiveMessage();
void laneFollowing(int targetColor);
void laneFollowingStraight(int targetColor);
#endif