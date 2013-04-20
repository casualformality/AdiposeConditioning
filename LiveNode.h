#ifndef LIVE_NODE
#define LIVE_NODE

#include "DisplayNode.h"
#include "Arduino.h"

#define CW  true
#define CCW false

class LiveNode : public DisplayNode {
public:
        LiveNode(ControllerIf*, DisplayNode*);
    virtual void    buttonUp();
    virtual void    buttonDn();
    virtual void    buttonLt();
    virtual void    buttonRt();
    virtual void    updateDisplay();
    void            start();
private:
    unsigned long   start_time;
    unsigned long   cur_time;
    char  valueStr[15];
    char  buffer[12];
};

#endif
