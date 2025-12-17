#include "functions.h"

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
