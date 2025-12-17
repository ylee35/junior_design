# Bad Cattitude Junior Design Project

Navigate our repository to power your Bad Cattitude robot! The main branch has all of our files, but you can find the main files for bot motions, color sensing, and object detection under the src folder. 

For the src folder:
Use main.cpp, functions.cpp, functions.h, def.h, & def.cpp to run the overall functionality of this bot. Use colorSensing.cpp & colorSensing.h for the color sensing functionality. Use motions.cpp & motions.h for the bot motions.

To access our secure server, run the client.py & app.py files under the main branch labeled "Websocket stuff". Run these in the terminal on your computer. Make sure you have Python & its websocket functionality installed. To run this server, run app.py first in one terminal & then run client.py in a separate terminal. 

(Note: The def.cpp files included in this repo reference a serverAddress variable that is the local IP address of our computer. To successfully run the server, change this IP address to the address of your personal computer.)

If you want to go beyond & use the LCD screen, navigate to the main branch to find the files regarding the final iiteration of the LCD Screen. The LCD Screen prints what state the bot is in.

For the LCD Screen:
The functions.cpp, functions.h, & def.h files are labeled "LCD Screen Code for States".
The main.cpp & platform.ini files are labeled "LCD Screen FINAL CODE using I2C".
The def.cpp file is labeled "LCD Screen WebSocket Working w Halligan Server".

Happy Roboting! Meow
