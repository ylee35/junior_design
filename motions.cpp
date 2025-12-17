#include "motions.h"

void BotMotions::stop() {
    // turn off motors
    digitalWrite(in1, LOW);
    digitalWrite(in2, LOW);
    digitalWrite(in3, LOW);
    digitalWrite(in4, LOW);
}

void BotMotions::backward() {
    digitalWrite(enA, HIGH);
    digitalWrite(enB, HIGH);

    // turn on motors
    digitalWrite(in1, LOW); // right motor
    digitalWrite(in3, LOW); // left motor
    digitalWrite(in2, HIGH); // right motor
    digitalWrite(in4, HIGH); // left motor

    analogWrite(enB, max_speed); // left motor enable
    analogWrite(enA, max_speed); // right motor enable
    delay(20);
}

void BotMotions::forward() {
    // Serial.println("in forward");
    digitalWrite(enA, HIGH);
    digitalWrite(enB, HIGH);

    // turn on motors
    digitalWrite(in1, HIGH); // right motor
    digitalWrite(in3, HIGH); // left motor
    digitalWrite(in2, LOW); // right motor
    digitalWrite(in4, LOW); // left motor

    analogWrite(enB, max_speed);
    analogWrite(enA, max_speed);
    delay(20);
}

void BotMotions::pivot_cc() {
    digitalWrite(enA, HIGH);
    digitalWrite(enB, HIGH);

    // turn on motors
    digitalWrite(in1, LOW); // right motor
    digitalWrite(in3, HIGH); // left motor
    digitalWrite(in2, LOW); // right motor
    digitalWrite(in4, LOW); // left motor

    analogWrite(enB, max_speed);
    analogWrite(enA, max_speed);
    delay(20);
}

void BotMotions::pivot_c() {
    digitalWrite(enA, HIGH);
    digitalWrite(enB, HIGH);

    // turn on motors
    digitalWrite(in1, HIGH); // right motor
    digitalWrite(in3, LOW); // left motor
    digitalWrite(in2, LOW); // right motor
    digitalWrite(in4, LOW); // left motor

    analogWrite(enB, max_speed);
    analogWrite(enA, max_speed);
    delay(20);
}

void BotMotions::right_turn() {
    // LEFT SPEED HAS TO BE HIGHER 
    digitalWrite(enA, HIGH);
    digitalWrite(enB, HIGH);

    // turn on motors
    digitalWrite(in1, HIGH); // right motor
    digitalWrite(in3, HIGH); // left motor
    digitalWrite(in2, LOW); // right motor
    digitalWrite(in4, LOW); // left motor

    analogWrite(enB, left_speed);
    analogWrite(enA, right_speed);
    delay(20);
}

void BotMotions::left_turn() {
    //RIGHT SPEED HAS TO BE HIGHER
    //Note: left motor is weak, so we need to have at least 50

    digitalWrite(enA, HIGH);
    digitalWrite(enB, HIGH);

    // turn on motors
    digitalWrite(in1, HIGH); // right motor
    digitalWrite(in3, HIGH); // left motor
    digitalWrite(in2, LOW); // right motor
    digitalWrite(in4, LOW); // left motor

    analogWrite(enB, left_speed);
    analogWrite(enA, right_speed);
    delay(20);
}

void BotMotions::repositionRight(){
    digitalWrite(enA, HIGH);
    digitalWrite(enB, HIGH);

    digitalWrite(in3, LOW); // left motor
    digitalWrite(in4, HIGH); // left motor
    digitalWrite(in1, HIGH); // right motor
    digitalWrite(in2, LOW); // right motor

    analogWrite(enB, max_speed);
    analogWrite(enA, max_speed);
    delay(20);

}

void BotMotions::repositionLeft(){
    digitalWrite(enA, HIGH);
    digitalWrite(enB, HIGH);

    digitalWrite(in3, HIGH); // left motor
    digitalWrite(in4, LOW); // left motor
    digitalWrite(in1, LOW); // right motor
    digitalWrite(in2, HIGH); // right motor

    analogWrite(enB, max_speed);
    analogWrite(enA, max_speed);
    delay(20);
}

void BotMotions::forwardLeft() {
    digitalWrite(enA, HIGH);
    digitalWrite(enB, HIGH);

    // turn on motors
    digitalWrite(in1, HIGH); // right motor
    digitalWrite(in3, HIGH); // left motor
    digitalWrite(in2, LOW); // right motor
    digitalWrite(in4, LOW); // left motor

    analogWrite(enB, max_speed + 50);
    analogWrite(enA, max_speed - 50);
    delay(20);
}

void BotMotions::forwardRight() {
    digitalWrite(enA, HIGH);
    digitalWrite(enB, HIGH);

    // turn on motors
    digitalWrite(in1, HIGH); // right motor
    digitalWrite(in3, HIGH); // left motor
    digitalWrite(in2, LOW); // right motor
    digitalWrite(in4, LOW); // left motor

    analogWrite(enB, max_speed - 50);
    analogWrite(enA, max_speed + 50);
    delay(20);
}

void BotMotions::set_speeds(int left_speed, int right_speed, int max_speed) {
    this->left_speed = left_speed;
    this->right_speed = right_speed;
    this->max_speed = max_speed;
}

