#include "SettingsNode.h"
#include <glcd.h>
#include "ControllerIf.h"

    SettingsNode::SettingsNode(ControllerIf *ctrl, const char *title, 
        DisplayNode *parent, const char *units, int *value, 
        int min_value = 0, int max_value = 1024) 
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
    sprintf(valueStr, "%.4d", tempValue);

    GLCD.ClearScreen();
    GLCD.CursorTo(0, 0);
    GLCD.print(this->toString());
    GLCD.CursorTo(0, 2);
    GLCD.print(tempValue);
    GLCD.CursorTo(0, 3);
    GLCD.print(units);
    GLCD.CursorTo(120, 1);
    GLCD.print("^");
    GLCD.CursorTo(120, 5);
    GLCD.print("`");
    GLCD.CursorTo(0, 7);
    GLCD.print("Set              Back");
}
