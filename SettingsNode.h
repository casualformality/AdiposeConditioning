#ifndef SETTINGS_NODE
#define SETTINGS_NODE

#include "DisplayNode.h"
#include "Arduino.h"

class SettingsNode : public DisplayNode {
public:
        SettingsNode(ControllerIf*, const char*, 
            DisplayNode*, const char*, int*, int, int);
    virtual void    buttonUp();
    virtual void    buttonDn();
    virtual void    buttonLt();
    virtual void    buttonRt();
    virtual void    updateDisplay();
private:
    int     tempValue;
    int     *valuePtr;
    int     max_value;
    int     min_value;
    char    valueStr[15];
    const char    *units;
};

#endif
