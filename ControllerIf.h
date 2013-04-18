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
#define SETTLE_ANGLE 5
#define FILL_ANGLE 50
#define CENTER_ANGLE 50
#define DETECTOR_PIN 0
#define PUMP_DIR_PIN 10
#define PUMP_STEP_PIN 9
#define PUMP_EN_PIN 8
#define STATE_WAIT_TIME 2000

#define VALVE_1_CLOSED  45
#define VALVE_1_HYP     0
#define VALVE_1_NORM    170
#define VALVE_2_CLOSED  45
#define VALVE_2_ADIPOSE 0
#define VALVE_2_WASTE   170

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
    int         getWeight();
    int         getCurWeight();
    int         getDetectorVal();
private:
    DisplayNode *currentNode;
    DisplayNode *rootNode;
    LiveNode    *liveNode;
    ErrorNode   *errorNode;
    
    void        pump();
    void        shake();
    bool        readDetector();
    void        updateValvePosition();
    
    int         update_interval;
    int         last_update;
    
    bool        buttonUpState;
    bool        buttonDnState;
    bool        buttonLtState;
    bool        buttonRtState;
    
    DeviceState cur_state;
    DeviceState nxt_state;
    int         cur_shake_angle;
    int         prv_shake_angle;
    bool        shaker_direction;
    bool        isFlushCycle;
    int         curFlush;
    ValvePos    valve_state;
    int         curIters;
    
    int         current_time;
    int         state_start_time;
    int         cur_state_delay;
    int         cur_check_delay;
    int         max_check_delay;
    int         cur_fill_weight;
    
    Servo       shake_servo;
    Servo       valve_1_servo;
    Servo       valve_2_servo;
    
    int         start_time;
    int         max_iterations;
    int         max_shake_time;
    int         max_flush_cycles;
    
    int         max_shaker_delay;
    int         max_shaker_angle;
    int         max_settle_time;
    int         max_drain_time;
    int         max_fill_delay;
    int         set_fill_weight;
    int         max_fill_weight;
    int         detector_threshold;
};

#endif
