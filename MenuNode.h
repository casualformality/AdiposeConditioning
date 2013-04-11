#ifndef MENU_NODE
#define MENU_NODE

#include "DisplayNode.h"

class MenuNode : public DisplayNode {
public:
            MenuNode(ControllerIf*, const char*, DisplayNode*, int);
    virtual void    buttonUp();
    virtual void    buttonDn();
    virtual void    buttonLt();
    virtual void    buttonRt();
    virtual void    updateDisplay();
private:
    void    selectItem(int);

    int     selectedItem;
    int     viewIndex;
};

#endif
