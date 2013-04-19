#include "LiveNode.h"
#include <glcd.h>
#include "ControllerIf.h"
#include "Arduino.h"

        LiveNode::LiveNode(ControllerIf *ctrl, DisplayNode *parent) 
            : DisplayNode(ctrl, "Start                ", parent, 0) {
    start_time = 0;
    cur_time = 0;
}

void LiveNode::updateDisplay() {
    cur_time = millis();
  
    GLCD.ClearScreen();
    
    GLCD.CursorTo(0, 0);
    switch(ctrl->getDeviceState()) {
        case MENU_STATE:
            GLCD.print("Starting...          ");
        case FILL_HYP:
            GLCD.print("Filling Hypoxic Sln. ");
            GLCD.CursorTo(0, 2);
            sprintf(buffer, "%.3d                  ", ctrl->getCurWeight());
            GLCD.print(buffer);
        break;
        case SHAKE_HYP:
            GLCD.print("Shaking Hypoxic Sln. ");
        break;
        case SETTLE_HYP:
            GLCD.print("Hypoxic Settling     ");
        break;
        case DRAIN_HYP:
            GLCD.print("Hypoxic Draining     ");
            GLCD.CursorTo(0, 2);
            sprintf(buffer, "%.3d                  ", ctrl->getCurWeight());
            GLCD.print(buffer);
            GLCD.CursorTo(0, 3);
            sprintf(buffer, "%.3d                  ", ctrl->getDetectorVal());
            GLCD.print(buffer);
        break;
        case FILL_NORM:
            GLCD.print("Filling Normoxic Sln.");
            GLCD.CursorTo(0, 2);
            sprintf(buffer, "%.3d                 ", ctrl->getCurWeight());
            GLCD.print(buffer);
        break;
        case SHAKE_NORM:
            GLCD.print("Shaking Normoxic Sln.");
        break;
        case SETTLE_NORM:
            GLCD.print("Normoxic Settling    ");
        break;
        case DRAIN_NORM:
            GLCD.print("Normoxic Draining    ");
            GLCD.CursorTo(0, 2);
            sprintf(buffer, "Weight: %.3d          ", ctrl->getCurWeight());
            GLCD.print(buffer);
            GLCD.CursorTo(0, 4);
            sprintf(buffer, "Fat Detector: %.3d    ", ctrl->getDetectorVal());
            GLCD.print(buffer);
        break;
        default:
            buttonRt();
            return;
        break;
    }

    sprintf(valueStr, "%.4d                 ", (cur_time - start_time)/1000);

    
    GLCD.CursorTo(0, 4);
    GLCD.print(valueStr);

    //GLCD.CursorTo(20, 1);
    //GLCD.print("^");
    //GLCD.CursorTo(20, 5);
    //GLCD.print("`");
    GLCD.CursorTo(0, 7);
    GLCD.print("                 Stop");
}

void LiveNode::start() {
    start_time = millis();
}

void LiveNode::buttonUp() {
    // No-op
}

void LiveNode::buttonDn() {
    // 
    // No-op
}

void LiveNode::buttonLt() {
        
}

void LiveNode::buttonRt() {
    // Handle cancel code
    ctrl->select(parent);
}
