#include <Arduino.h>


#define motor1L 6
#define motor1R 7
#define motor2L 8
#define motor2R 9


void setup() {
 Serial.begin(9600);
 Serial.println("serial begun");
 pinMode (motor1L , OUTPUT);
 pinMode (motor1R , OUTPUT);
 pinMode (motor2L , OUTPUT);
 pinMode (motor2R , OUTPUT);
}


void loop() {
 digitalWrite(motor1L , HIGH);
 digitalWrite(motor2L , HIGH);
 delay(5000);
 digitalWrite(motor1L , LOW);
 digitalWrite(motor1R , HIGH);
 digitalWrite(motor2L , LOW);
 digitalWrite(motor2R , HIGH);
 delay(5000);
 digitalWrite(motor1R , LOW);
 digitalWrite(motor2R , LOW);


}
