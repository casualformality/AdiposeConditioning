#include "SettingsNode.h"
#include <glcd.h>
#include "ControllerIf.h"
#include "Arduino.h"

    SettingsNode::SettingsNode(ControllerIf *ctrl, const char *title, 
        DisplayNode *parent, const char *units, unsigned long *value, 
        unsigned long min_value = 0, unsigned long max_value = 1024) 
            : DisplayNode(ctrl, title, parent, 0) {
    if(units != NULL) {
        this->units = units;
    } else {
        this->units = "";
    }
    valuePtr = value;
    tempValue = *valuePtr;
    this->min_value = min_value;
    this->max_value = max_value;
}

void SettingsNode::buttonUp() {
    if(tempValue < max_value) {
        tempValue++;
        updateDisplay();
    }
}

void SettingsNode::buttonDn() {
    if(tempValue > min_value) {
        tempValue--;
        updateDisplay();
    }
}

// Set Value
void SettingsNode::buttonLt() {
    *valuePtr = tempValue;
    ctrl->select(parent);
}

// Undo change
void SettingsNode::buttonRt() {
    tempValue = *valuePtr;
    ctrl->select(parent);
}

void SettingsNode::updateDisplay() {
    sprintf(valueStr, "%.4lu                 ", tempValue);

    GLCD.ClearScreen();
    GLCD.CursorTo(0, 0);
    GLCD.print(this->toString());
    GLCD.CursorTo(0, 2);
    GLCD.print(valueStr);
    GLCD.CursorTo(0, 3);
    GLCD.print(units);
    //GLCD.CursorTo(20, 1);
    //GLCD.print("^");
    //GLCD.CursorTo(20, 5);
    //GLCD.print("`");
    GLCD.CursorTo(0, 7);
    GLCD.print("Set              Back");
}
