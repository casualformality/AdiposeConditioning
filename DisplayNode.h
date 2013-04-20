#ifndef DISPLAY_NODE_H
#define DISPLAY_NODE_H
#include "Arduino.h"

class ControllerIf;

#define MAX_INDEX 6

class DisplayNode {
public:
                    DisplayNode(ControllerIf*, const char*, DisplayNode*, unsigned long);
    virtual         ~DisplayNode();
    const char      *toString();
    DisplayNode     *getParent();
    void            setParent(DisplayNode*);
    void            addChild(DisplayNode*);
    unsigned long             getNumChildren();
    DisplayNode     *getChildAt(unsigned long);
    virtual void    buttonUp();
    virtual void    buttonDn();
    virtual void    buttonLt();
    virtual void    buttonRt();
    virtual void    updateDisplay();
protected:
    ControllerIf    *ctrl;
    const char      *title;
    DisplayNode     *parent;
    unsigned long             numChildren;
    unsigned long             childCount;
    DisplayNode     **children;
};

#endif
