#ifndef CONTROLLER_IF_H
#define CONTROLLER_IF_H

#include <Servo.h>
#include "Arduino.h"

class DisplayNode;
class LiveNode;
class ErrorNode;

#define BUTTON_UP_PIN 49
#define BUTTON_DN_PIN 48
#define BUTTON_LT_PIN 46
#define BUTTON_RT_PIN 47
#define VALVE_1_PIN 6
#define VALVE_2_PIN 5
#define SHAKE_PIN 3
#define WEIGHT_PIN 3
#define SETTLE_ANGLE 140
#define FILL_ANGLE 100
#define CENTER_ANGLE 50
#define DETECTOR_PIN 0
#define PUMP_DIR_PIN 10
#define PUMP_STEP_PIN 9
#define PUMP_EN_PIN 8
#define STATE_WAIT_TIME 2000

#define VALVE_1_CLOSED  45
#define VALVE_1_HYP     180
#define VALVE_1_NORM    0
#define VALVE_2_CLOSED  45
#define VALVE_2_ADIPOSE 0
#define VALVE_2_WASTE   180

#define DEBUG_TIME 0
#define DEBUG_PIN 13

enum DeviceState {
    MENU_STATE,
    FILL_HYP,
    SHAKE_HYP,
    SETTLE_HYP,
    DRAIN_HYP,
    FILL_NORM,
    SHAKE_NORM,
    SETTLE_NORM,
    DRAIN_NORM,
    DONE
};

enum ValvePos {
    CLOSED,
    HYP_OPEN,
    NORM_OPEN,
    DRAIN
};

class ControllerIf {
public:
                ControllerIf();
    void        start();
    void        select(DisplayNode*);
    DisplayNode *getCurrentNode();
    void        getUserInput();
    void        updateState();
    DeviceState getDeviceState();
    unsigned long         getWeight();
    unsigned long         getCurWeight();
    unsigned long         getDetectorVal();
private:
    DisplayNode *currentNode;
    DisplayNode *rootNode;
    LiveNode    *liveNode;
    ErrorNode   *errorNode;
    
    void        pump();
    void        shake();
    bool        readDetector();
    void        updateValvePosition();
    
    unsigned long         update_interval;
    unsigned long         last_update;
    
    bool        buttonUpState;
    bool        buttonDnState;
    bool        buttonLtState;
    bool        buttonRtState;
    
    DeviceState cur_state;
    DeviceState nxt_state;
    unsigned long         cur_shake_angle;
    unsigned long         prv_shake_angle;
    bool        shaker_direction;
    bool        isFlushCycle;
    unsigned long         curFlush;
    ValvePos    valve_state;
    unsigned long         curIters;
    
    unsigned long         current_time;
    unsigned long         state_start_time;
    unsigned long         cur_state_delay;
    unsigned long         cur_check_delay;
    unsigned long         max_check_delay;
    unsigned long         cur_fill_weight;
    
    Servo       shake_servo;
    Servo       valve_1_servo;
    Servo       valve_2_servo;
    
    unsigned long         start_time;
    unsigned long         max_iterations;
    unsigned long         max_shake_time;
    unsigned long         max_flush_cycles;
    
    unsigned long         max_shaker_delay;
    unsigned long         max_shaker_angle;
    unsigned long         max_settle_time;
    unsigned long         max_drain_time;
    unsigned long         fill_speed;
    unsigned long         set_fill_weight;
    unsigned long         max_fill_weight;
    unsigned long         detector_threshold;
};

#endif
