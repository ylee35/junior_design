

/*
 *      functions.h
 * 
 *      This file contains function declarations for all helper functions used. 
 *      It also defines the BotMotions class. 
 * 
*/

#include <def.h>

#ifndef FUNCTIONS_H
#define FUNCTIONS_H

void setPinModes();
void wifiSetup();
void sendMessage(String messageToTeam);
String receiveMessage();

#endif
