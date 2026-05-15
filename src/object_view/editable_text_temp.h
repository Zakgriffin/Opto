#pragma once

#include "globals.h"

typedef struct EditableTextTemp {
    string text;
    int character_index;
    Box box;
    ray::Color color;

    bool selected;

    unordered_map<char, function<bool()>> on_keys;

    EditableTextTemp();
    ~EditableTextTemp();

    float b(int character_index);
    
    void accept_input(InputContext* ctx);
    void layout();
    void draw();
} EditableTextTemp;
