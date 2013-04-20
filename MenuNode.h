#ifndef MENU_NODE
#define MENU_NODE

#include "DisplayNode.h"
#include "Arduino.h"

class MenuNode : public DisplayNode {
public:
            MenuNode(ControllerIf*, const char*, DisplayNode*, unsigned long);
    virtual void    buttonUp();
    virtual void    buttonDn();
    virtual void    buttonLt();
    virtual void    buttonRt();
    virtual void    updateDisplay();
private:
    void    selectItem(unsigned long);

    unsigned long     selectedItem;
    unsigned long     viewIndex;
};

#endif
