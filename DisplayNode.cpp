#include "DisplayNode.h"
#include "ControllerIf.h"
#include <stdlib.h>

DisplayNode::DisplayNode(ControllerIf *ctrl, const char *title, DisplayNode *parent, int numChildren) {
    this->title = title;
    this->parent = parent;
    this->numChildren = numChildren;
    this->ctrl = ctrl;
    childCount = 0;
    if(numChildren > 0) {
        children = (DisplayNode **)malloc(numChildren*sizeof(DisplayNode*));
    } else {
        children = NULL;
    }
}

DisplayNode::~DisplayNode() {
    if(children != NULL) {
        free(children);
    }
}

const char  *DisplayNode::toString() {
    return title;
}

DisplayNode *DisplayNode::getParent() {
    return parent;
}

void        DisplayNode::setParent(DisplayNode* node) {
    if(this != node) {
        parent = node;
    }
}

void        DisplayNode::addChild(DisplayNode* node) {
    if(this == node || node == NULL) {
        return;
    }

    int index = 0;

    if(childCount < numChildren) {
        index = childCount;
        childCount++;
    } else {
        index = 0;
    }

    children[index] = node;
}

int         DisplayNode::getNumChildren() {
    return childCount;
}

DisplayNode *DisplayNode::getChildAt(int index) {
    if(children != NULL && index >= 0 && index < childCount) {
        return children[index];
    } else {
        return NULL;
    }
}

void  DisplayNode::buttonUp() {}
void  DisplayNode::buttonDn() {}
void  DisplayNode::buttonLt() {}
void  DisplayNode::buttonRt() {}
void  DisplayNode::updateDisplay() {}

