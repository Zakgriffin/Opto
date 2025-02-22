
#ifndef OPTO_EDITABLE_TEXT_H
#define OPTO_EDITABLE_TEXT_H

#include "stds.h"
#include "globals.h"

class EditableText {
public:
    string text;
    int character_index;
    bool selected;
    Rectangle box;

    EditableText();

    void draw();
};

#endif //OPTO_EDITABLE_TEXT_H
