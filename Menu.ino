#include <glcd.h>
#include "fonts/SystemFont5x7.h"
#include <Servo.h>
#include "ControllerIf.h"
#include "DisplayNode.h"

ControllerIf *ctrl;

void setup() {
    pinMode(BUTTON_UP_PIN, INPUT);
    pinMode(BUTTON_DN_PIN, INPUT);
    pinMode(BUTTON_LT_PIN, INPUT);
    pinMode(BUTTON_RT_PIN, INPUT);

    GLCD.Init();
    GLCD.SelectFont(System5x7);
    GLCD.ClearScreen();
  
    ctrl = new ControllerIf();
    ctrl->start();
}

void loop() {
    ctrl->getUserInput();
    ctrl->updateState();
}
