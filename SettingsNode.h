#ifndef SETTINGS_NODE
#define SETTINGS_NODE

#include "DisplayNode.h"
#include "Arduino.h"

class SettingsNode : public DisplayNode {
public:
        SettingsNode(ControllerIf*, const char*, 
            DisplayNode*, const char*, unsigned long*, unsigned long, unsigned long);
    virtual void    buttonUp();
    virtual void    buttonDn();
    virtual void    buttonLt();
    virtual void    buttonRt();
    virtual void    updateDisplay();
private:
    unsigned long     tempValue;
    unsigned long     *valuePtr;
    unsigned long     max_value;
    unsigned long     min_value;
    char    valueStr[15];
    const char    *units;
};

#endif
