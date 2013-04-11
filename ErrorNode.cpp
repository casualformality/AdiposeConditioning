#include "ErrorNode.h"
#include <glcd.h>
#include "ControllerIf.h"

        ErrorNode::ErrorNode(ControllerIf *ctrl) 
            : DisplayNode(ctrl, "", NULL, 0) {
    errorMessage = "Undefined Error";
}

void ErrorNode::buttonUp() {
    /* No-op */
}

void ErrorNode::buttonDn() {
    /* No-op */
}

void ErrorNode::buttonLt() {
    /* No-op */
}

void ErrorNode::buttonRt() {
    ctrl->select(parent);
}

void ErrorNode::setErrorMessage(const char *c) {
    if(c != NULL) {
        errorMessage = c;
    }
}

void ErrorNode::updateDisplay() {
    GLCD.ClearScreen();
    GLCD.CursorTo(0, 0);
    GLCD.print(errorMessage);
    GLCD.CursorTo(0, 7);
    GLCD.print("                   OK");
}
