#ifndef ERROR_NODE
#define ERROR_NODE

#include "DisplayNode.h"

class ErrorNode : public DisplayNode {
public:
        ErrorNode(ControllerIf*);
    virtual void    buttonUp();
    virtual void    buttonDn();
    virtual void    buttonLt();
    virtual void    buttonRt();
    virtual void    updateDisplay();
    void            setErrorMessage(const char *);
private:
    const char  *errorMessage;
};

#endif
