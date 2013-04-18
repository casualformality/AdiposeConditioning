#include "MenuNode.h"
#include <glcd.h>
#include "ControllerIf.h"
#include "Arduino.h"

        MenuNode::MenuNode(ControllerIf *ctrl, const char *title, DisplayNode *parent, 
        int numChildren) : DisplayNode(ctrl, title, parent, numChildren) {
    selectedItem = 0;
    viewIndex = 0;
}

void    MenuNode::selectItem(int index) {
    selectedItem = index;
    if(index <= viewIndex + MAX_INDEX && index >= viewIndex) {
        GLCD.InvertRect(0, (selectedItem - viewIndex) * 8, 117, 7);
    }
}

void MenuNode::updateDisplay() {
    int i = 0;

    GLCD.ClearScreen();
    GLCD.CursorTo(20, 1);
    GLCD.print("^");
    GLCD.CursorTo(20, 5);
    GLCD.print("`");
    GLCD.CursorTo(0, 7);
    GLCD.print("Select           Back");
    
    for(int index = viewIndex; index < viewIndex + MAX_INDEX + 1 && index < getNumChildren(); index++) {
        GLCD.CursorTo(0, i);
        DisplayNode *child = getChildAt(index);
        if(child != NULL) {
            GLCD.print(child->toString());
        }
        i++;
    }
    
    selectItem(selectedItem);
}

// Up
void MenuNode::buttonUp() {
    if(getNumChildren() == 0) {
        return;
    } else if(selectedItem == 0) {
        selectedItem = -1;
        viewIndex = getNumChildren() - MAX_INDEX - 1;
        if(viewIndex < 0) {
            viewIndex = 0; 
            updateDisplay();
        }
        selectItem(getNumChildren() - 1);
    } else if(selectedItem == viewIndex) {
        selectedItem = -1;
        viewIndex--;
        updateDisplay();
        selectItem(viewIndex);
    } else {
        selectItem(selectedItem - 1);
    }
}

// Down
void MenuNode::buttonDn() {
    if(getNumChildren() == 0) {
        return;
    } else if(selectedItem == getNumChildren() - 1) {
        selectedItem = -1;
        viewIndex = 0;
        updateDisplay();
        selectItem(0);
    } else if(selectedItem == viewIndex + MAX_INDEX) {
        selectedItem = -1;
        viewIndex++;
        updateDisplay();
        selectItem(viewIndex + MAX_INDEX);
    } else {
        selectItem(selectedItem + 1);
    }
}

// Back
void MenuNode::buttonLt() {
    ctrl->select(getChildAt(selectedItem));
}

// Select
void MenuNode::buttonRt() {
    ctrl->select(parent);
}
