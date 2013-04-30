#include "ControllerIf.h"
#include "MenuNode.h"
#include "SettingsNode.h"
#include "LiveNode.h"
#include "ErrorNode.h"
#include "Arduino.h"

#include <glcd.h>
#include <Serial.h>

boolean debug_state;

ControllerIf::ControllerIf() {
    last_update = 0;
    current_time = 0;
    update_interval = 500;
    debug_state = true;

    // Default User-Settable Values
    max_iterations = 2;
    max_shake_time = 20;
    max_flush_cycles = 1;
    max_shaker_delay = 10;
    max_shaker_angle = 50;
    max_settle_time = 30;
    fill_speed = 100;
    max_fill_weight = 20;
    detector_threshold = 180;
    max_check_delay = 500;
    cur_fill_weight = 0;

    errorNode = new ErrorNode(this);
    rootNode = new MenuNode(this, "Welcome!", NULL, 3);
    
    liveNode = new LiveNode(this, rootNode);
    rootNode->addChild(liveNode);
    
    DisplayNode *runSettings = new MenuNode(this, "Run Settings", rootNode, 4);
    rootNode->addChild(runSettings);
    DisplayNode *node = new SettingsNode(this, "Cycle Count", runSettings, "", &max_iterations, 0, 100);
    runSettings->addChild(node);
    node = new SettingsNode(this, "Shake Time", runSettings, "Seconds", &max_shake_time, 0, 300);
    runSettings->addChild(node);
    node = new SettingsNode(this, "Fill Volume", runSettings, "Grams", &max_fill_weight, 0, 100);
    runSettings->addChild(node);
    node = new SettingsNode(this, "Flush Cycles", runSettings, "", &max_flush_cycles, 0, 10);
    runSettings->addChild(node);
    
    DisplayNode *devSettings = new MenuNode(this, "Device Settings", rootNode, 8);
    rootNode->addChild(devSettings);
    node = new SettingsNode(this, "Pump Speed", devSettings, "% of Full Speed", &fill_speed, 0, 100);
    devSettings->addChild(node);
    node = new SettingsNode(this, "Shaker Speed", devSettings, "MS Wait", &max_shaker_delay, 0, 1000);
    devSettings->addChild(node);
    node = new SettingsNode(this, "Shaker Angle", devSettings, "Degrees from Horizon", &max_shaker_angle, 0, 50);
    devSettings->addChild(node);
    node = new SettingsNode(this, "Settle Time", devSettings, "Seconds", &max_settle_time, 0, 300);
    devSettings->addChild(node);
    node = new SettingsNode(this, "Detector Threshold", devSettings, "0-1024", &detector_threshold, 0, 1024);
    devSettings->addChild(node);

    shake_servo.attach(SHAKE_PIN);
    valve_1_servo.attach(VALVE_1_PIN);
    valve_2_servo.attach(VALVE_2_PIN);
    pinMode(PUMP_DIR_PIN, OUTPUT);
    pinMode(PUMP_STEP_PIN, OUTPUT);
    pinMode(PUMP_EN_PIN, OUTPUT);
    pinMode(DEBUG_PIN, OUTPUT);

    select(rootNode);
}

void        ControllerIf::start() {
    rootNode->updateDisplay();
    
    isFlushCycle = false;
    curFlush = 0;
    curIters = 0;
    cur_state_delay = 0;
    state_start_time = 0;
    current_time = 0;
    shaker_direction = true;
    cur_shake_angle = 5;
    prv_shake_angle = -1;

    valve_1_servo.write(VALVE_1_CLOSED);
    valve_2_servo.write(VALVE_2_CLOSED);
    digitalWrite(PUMP_DIR_PIN, LOW);
    digitalWrite(PUMP_EN_PIN, LOW);
    
    if(DEBUG_TIME > 0) {
      select(liveNode);
    } else {
      cur_state = MENU_STATE;
    }
}

unsigned long        ControllerIf::getDetectorVal() {
    return analogRead(DETECTOR_PIN);
}

bool        ControllerIf::readDetector() {
    if(analogRead(DETECTOR_PIN) > detector_threshold) {
        return true;
    } else {
        return false;
    }
}

void        ControllerIf::pump() {
  unsigned long steps = 360.0 / 0.225;
  float usDelay = (1 / (fill_speed / 100.0)) * 70.0;
  
  for(unsigned long i = 0; i < steps; i++) {
      digitalWrite(PUMP_STEP_PIN, HIGH);
      delayMicroseconds(usDelay); 

      digitalWrite(PUMP_STEP_PIN, LOW);
      delayMicroseconds(usDelay);
  }
}

void        ControllerIf::shake() {
  if(prv_shake_angle != cur_shake_angle) {
      prv_shake_angle = cur_shake_angle;
      shake_servo.write(cur_shake_angle);
  }
}

void        ControllerIf::updateValvePosition() {
    switch(cur_state) {
        case MENU_STATE:
        case SHAKE_HYP:
        case SETTLE_HYP:
        case SHAKE_NORM:
        case SETTLE_NORM:
        case DONE:
            if(valve_state != CLOSED) {
                valve_state = CLOSED;
                valve_1_servo.write(VALVE_1_CLOSED);
                valve_2_servo.write(VALVE_2_CLOSED);
            }
            break;
        case FLUSH_SLN:
            if(valve_state != FLUSH) {
                valve_state = FLUSH;
                valve_1_servo.write(VALVE_1_HYP);
                valve_2_servo.write(VALVE_2_FLUSH);
            }
            break;
        case FILL_HYP:
            if(valve_state != HYP_OPEN) {
                valve_state = HYP_OPEN;
                valve_1_servo.write(VALVE_1_HYP);
                valve_2_servo.write(VALVE_2_ADIPOSE);
            }
            break;
        case FILL_NORM:
            if(valve_state != NORM_OPEN) {
                valve_state = NORM_OPEN;
                valve_1_servo.write(VALVE_1_NORM);
                valve_2_servo.write(VALVE_2_ADIPOSE);
            }
            break;
        case DRAIN_HYP:
        case DRAIN_NORM:
            if(valve_state != DRAIN) {
                valve_state = DRAIN;
                valve_1_servo.write(VALVE_1_CLOSED);
                valve_2_servo.write(VALVE_2_WASTE);
            }
            break;
    }
}

void        ControllerIf::select(DisplayNode *node) {
    if(currentNode == liveNode) {
        cur_state = MENU_STATE;
        updateValvePosition();
    }
    
    if(node == NULL) {
        currentNode = rootNode;
    } else {
        currentNode = node;
    }
    
    if(currentNode == liveNode) {
        liveNode->start();
    }

    if(currentNode != NULL) {
        currentNode->updateDisplay();
    }
}

DisplayNode *ControllerIf::getCurrentNode() {
    return currentNode;
}

void        ControllerIf::getUserInput() {
    if(digitalRead(BUTTON_UP_PIN) == 0) {
        if(!buttonUpState) {
            buttonUpState = true;
            getCurrentNode()->buttonUp();
            GLCD.InvertRect(119, 7, 6, 7);
        }
    } else {
        if(buttonUpState) {
            buttonUpState = false;
            getCurrentNode()->updateDisplay();
        }
    }
    if(digitalRead(BUTTON_DN_PIN) == 0) {
        if(!buttonDnState) {
            buttonDnState = true;
            getCurrentNode()->buttonDn();
            GLCD.InvertRect(119, 38, 6, 7);
        }
    } else {
        if(buttonDnState) {
            buttonDnState = false;
            getCurrentNode()->updateDisplay();
        }
    }
    if(digitalRead(BUTTON_LT_PIN) == 0) {
        if(!buttonLtState) {
            buttonLtState = true;
            getCurrentNode()->buttonLt();
            GLCD.InvertRect(0, 56, 63, 7);
        }
    } else {
        if(buttonLtState) {
            buttonLtState = false;
            getCurrentNode()->updateDisplay();
        }
    }
    if(digitalRead(BUTTON_RT_PIN) == 0) {
        if(!buttonRtState) {
            buttonRtState = true;
            getCurrentNode()->buttonRt();
            GLCD.InvertRect(64, 56, 63, 7);
        }
    } else {
        if(buttonRtState) {
            buttonRtState = false;
            getCurrentNode()->updateDisplay();
        }
    }
    
    if(currentNode == liveNode) {
        unsigned long current_time = millis();
        if(current_time >= last_update + update_interval) {
            last_update = current_time;
            getCurrentNode()->updateDisplay();
        }
    }
}

DeviceState ControllerIf::getDeviceState() {
    return cur_state;
}

unsigned long ControllerIf::getWeight() {
    unsigned long tmp = analogRead(WEIGHT_PIN);
    tmp = (tmp - set_fill_weight) * 1.2;
    if(tmp >= 0) {
        cur_fill_weight = (cur_fill_weight * 0.85) + (tmp * 0.15);
    }
    return cur_fill_weight;
}

unsigned long ControllerIf::getCurWeight() {
    return cur_fill_weight;
}

void ControllerIf::updateState() {
    // Don't do anything if we're not in a running state
    if(currentNode != liveNode) {
        return;
    }
    
    // Each state needs some lead-in delay time for servo movement, etc.
    current_time = millis();
    if(current_time < (state_start_time + STATE_WAIT_TIME)) {
        return;
    }
    
    // Check weight at a regular interval
    if(current_time > cur_check_delay) {
        cur_check_delay += max_check_delay;
        getWeight();
    }
    
    switch(cur_state) {
        case MENU_STATE:
            set_fill_weight = analogRead(WEIGHT_PIN);
            cur_fill_weight = 0;
            cur_shake_angle = FILL_ANGLE;
            nxt_state = FILL_HYP;
            Serial.println("Switching to FILL_HYP");
        break;
        case FLUSH_SLN:
            if(current_time < (state_start_time + 5000 + STATE_WAIT_TIME)) {
                pump();
            } else {
                nxt_state = FILL_HYP;
                Serial.println("Switching to FILL_HYP");
            }
        break;
        case FILL_HYP:
            if((DEBUG_TIME != 0) 
                    && (current_time < (state_start_time + DEBUG_TIME + STATE_WAIT_TIME))) {
                pump();
            } else if(DEBUG_TIME == 0 
                    && (cur_fill_weight < max_fill_weight)) {
                pump();
            } else {
                if(isFlushCycle) {
                    if(curFlush < max_flush_cycles) {
                        curFlush++;
                        Serial.println("Switching to DRAIN_HYP");
                        nxt_state = DRAIN_HYP;
                    } else {
                        isFlushCycle = false;
                        curFlush = 0;
                        Serial.println("Switching to SHAKE_HYP");
                        nxt_state = SHAKE_HYP;
                    }
                } else {
                    Serial.println("Switching to SHAKE_HYP");
                    nxt_state = SHAKE_HYP;
                }
            }
        break;
        case SHAKE_HYP:
            if(current_time < (state_start_time + (max_shake_time * 1000) + STATE_WAIT_TIME)) {
                if(cur_shake_angle == SETTLE_ANGLE) {
                    shaker_direction = true;
                    cur_shake_angle = CENTER_ANGLE - max_shaker_angle;
                    shake();
                }
                if(current_time >= cur_state_delay) {
                    cur_state_delay += max_shaker_delay;
                    if(shaker_direction) {
                        cur_shake_angle++;
                        if(cur_shake_angle >= (CENTER_ANGLE + max_shaker_angle)) {
                            shaker_direction = false;
                        }
                    } else {
                        cur_shake_angle--;
                        if(cur_shake_angle <= (CENTER_ANGLE - max_shaker_angle)) {
                            shaker_direction = true;
                        }
                    }
                    shake();
                }
            } else {
                Serial.println("Switching to SETTLE_HYP");
                cur_shake_angle = SETTLE_ANGLE;
                shake();
                nxt_state = SETTLE_HYP;
            }
        break;
        case SETTLE_HYP:
            if(current_time < (state_start_time + (max_settle_time * 1000) + STATE_WAIT_TIME)) {
                // No-op
            } else {
                Serial.println("Switching to DRAIN_HYP");
                nxt_state = DRAIN_HYP;
            }
        break;
        case DRAIN_HYP:
            if(DEBUG_TIME != 0 
                    && (current_time <= (state_start_time + DEBUG_TIME + STATE_WAIT_TIME))
                    && (!readDetector())) {
                /* No-op */
            } else if(!readDetector() 
                    && (cur_fill_weight > 1) 
                    && (DEBUG_TIME == 0) 
                    // Set a max drain time
                    && (current_time < (state_start_time + 12000))) {
                /* No-op */
            } else {
                if(isFlushCycle) {
                    Serial.println("Switching to FILL_HYP");
                    nxt_state = FILL_HYP;
                } else {
                    Serial.println("Switching to FILL_NORM");
                    nxt_state = FILL_NORM;
                }
                cur_shake_angle = FILL_ANGLE;
                shake();
            }
        break;
        case FILL_NORM:
            if((DEBUG_TIME != 0) 
                    && (current_time < (state_start_time + DEBUG_TIME + STATE_WAIT_TIME))) {
                    pump();
            } else if((DEBUG_TIME == 0) 
                    && (cur_fill_weight < max_fill_weight)) {
                /* No-op */
                pump();
            } else {
                Serial.println("Switching to SHAKE_NORM");
                nxt_state = SHAKE_NORM;
            }
        break;
        case SHAKE_NORM:
            if(current_time < (state_start_time + (max_shake_time * 1000) + STATE_WAIT_TIME)) {
                if(cur_shake_angle == SETTLE_ANGLE) {
                    shaker_direction = true;
                    cur_shake_angle = CENTER_ANGLE - max_shaker_angle;
                    shake();
                }
                if(current_time >= cur_state_delay) {
                    cur_state_delay += max_shaker_delay;
                    if(shaker_direction) {
                        cur_shake_angle++;
                        if(cur_shake_angle >= CENTER_ANGLE + max_shaker_angle) {
                            shaker_direction = false;
                        }
                    } else {
                        cur_shake_angle--;
                        if(cur_shake_angle <= CENTER_ANGLE - max_shaker_angle) {
                            shaker_direction = true;
                        }
                    }
                    shake();
                }
            } else {
                Serial.println("Switching to SETTLE_NORM");
                cur_shake_angle = SETTLE_ANGLE;
                shake();
                nxt_state = SETTLE_NORM;
            }
        break;
        case SETTLE_NORM:
            if(current_time < (state_start_time + (max_settle_time * 1000))) {
                // No-op
            } else {
                Serial.println("Switching to DRAIN_NORM");
                nxt_state = DRAIN_NORM;
            }
        break;
        case DRAIN_NORM:
            if(DEBUG_TIME != 0 
                    && (current_time <= (state_start_time + DEBUG_TIME + STATE_WAIT_TIME))
                    && (!readDetector())) {
            /* No-op */
            } else if(!readDetector() 
                    && (cur_fill_weight > 1) 
                    && (DEBUG_TIME == 0) 
                    // Set a max drain time
                    && (current_time < (state_start_time + 12000))) {
                /* No-op */
            } else {
                if(curIters < max_iterations) {
                    isFlushCycle = true;
                    curIters++;
                    Serial.println("Switching to FILL_HYP");
                    cur_shake_angle = FILL_ANGLE;
                    nxt_state = FILL_HYP;
                } else {
                    Serial.println("Switching to DONE");
                    cur_shake_angle = SETTLE_ANGLE;
                    nxt_state = DONE;
                }
                shake();
            }
        break;
        case DONE:
            // No-op
        break;
    }

    if(cur_state != nxt_state) {
        cur_state = nxt_state;
        updateValvePosition();
        state_start_time = millis();
        cur_check_delay = state_start_time;
        cur_state_delay = state_start_time;
        if((cur_state == FILL_HYP) || (cur_state == FILL_NORM) || (cur_state == FLUSH_SLN)) {
            digitalWrite(PUMP_EN_PIN, HIGH);
        } else {
            digitalWrite(PUMP_EN_PIN, LOW);
        }
    }
}
