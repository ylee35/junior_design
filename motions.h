#ifndef MOTIONS_H
#define MOTIONS_H

#include "def.h"

class BotMotions {
    public: 
        void directionControl();
        void speedControl();
        void forward();
        void forwardLeft();
        void forwardRight();
        void stop();
        void backward();
        void pivot_c();
        void pivot_cc();
        void right_turn();
        void left_turn();
        void repositionLeft();
        void repositionRight();
        void set_speeds(int left_speed, int right_speed, int max_speed);
    
    private: 
        int left_speed;
        int right_speed;
        int max_speed;
};

#endif