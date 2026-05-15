#include "editable_text_temp.h"
#include "globals.h"
#include "raylib.h"

static constexpr int PAD_X = 5;
static constexpr int PAD_Y = 3;

unordered_set<EditableTextTemp*> editable_texts;

EditableTextTemp::EditableTextTemp() {
    this->character_index = 0;
    this->selected = false;
    this->text_signaled = false;

    editable_texts.insert(this);
}

EditableTextTemp::~EditableTextTemp() {
    editable_texts.erase(this);
}

float EditableTextTemp::b(int character_index) {
    return this->box.x_min + CHARACTER_WIDTH * character_index + PAD_X;
}

enum Direction { LEFT, RIGHT, UP, DOWN };

void jump_editable_text(EditableTextTemp* e, Direction dir) {
    Box search = e->box;
    if      (dir == LEFT)  { search.x_max = search.x_min; search.x_min = -FLT_MAX; }
    else if (dir == RIGHT) { search.x_min = search.x_max; search.x_max =  FLT_MAX; }
    else if (dir == UP)    { search.y_max = search.y_min; search.y_min = -FLT_MAX; }
    else if (dir == DOWN)  { search.y_min = search.y_max; search.y_max =  FLT_MAX; }

    EditableTextTemp* best = nullptr;
    for (auto et : editable_texts) {
        if (et == e) continue;
        if (!boxes_overlap(et->box, search)) continue;
        if (best == nullptr) { best = et; continue; }

        bool closer = false;
        if      (dir == LEFT)  closer = et->box.x_max > best->box.x_max;
        else if (dir == RIGHT) closer = et->box.x_min < best->box.x_min;
        else if (dir == UP)    closer = et->box.y_max > best->box.y_max;
        else if (dir == DOWN)  closer = et->box.y_min < best->box.y_min;
        if (closer) best = et;
    }
    if (!best) return;

    e->selected = false;
    best->selected = true;

    float x = e->b(e->character_index);
    int end = (int) best->text.size();
    auto mapped = map_range<float>(x, best->b(0), best->b(end), 0.5f, (float)end + 0.5f);
    best->character_index = clamp((int) round(mapped), 0, end);
}

void unselect_all_texts() {
    for(auto e : editable_texts) {
        e->selected = false;
    }
}

// ZZZZ this should be not in here
void enter_assemble_mode() {
    unselect_all_texts();
    edit_mode = ASSEMBLE_MODE;
}

void select_text(EditableTextTemp* e) {
    unselect_all_texts();
    edit_mode = TEXT_MODE;
    e->selected = true;
}

void EditableTextTemp::accept_input(InputContext* ctx) {
    if (any_click(ctx, this->box)) {
        if(
            consume_click_streak_if(ctx, 2, edit_mode == ASSEMBLE_MODE) ||
            consume_click_streak_if(ctx, 1, edit_mode == TEXT_MODE)
        ) {
            select_text(this);
        }
    }

    if (edit_mode == TEXT_MODE && this->selected) {
        if (consume(&ctx->key, ray::KEY_LEFT)) {
            if (this->character_index > 0) this->character_index--;
            else jump_editable_text(this, LEFT);
        } else if (consume(&ctx->key, ray::KEY_RIGHT)) {
            if (this->character_index < this->text.size()) this->character_index++;
            else jump_editable_text(this, RIGHT);
        } else if (consume(&ctx->key, ray::KEY_UP)) {
            jump_editable_text(this, UP);
        } else if (consume(&ctx->key, ray::KEY_DOWN)) {
            jump_editable_text(this, DOWN);
        } else if (consume(&ctx->key, ray::KEY_SPACE)) {
            jump_editable_text(this, RIGHT);
        } else if (consume(&ctx->key, ray::KEY_ESCAPE)) {
            enter_assemble_mode();
        } else if (consume(&ctx->key, ray::KEY_BACKSPACE)) {
            if (this->text.length() > 0 && this->character_index > 0) {
                this->character_index--;
                this->text.erase(this->character_index, 1);
                
                this->text_signaled = true;
            } 
        } else if (ctx->ch > 0) {
            this->text.insert(this->character_index, 1, (char)ctx->ch);
            this->character_index++;
            ctx->ch = 0;
            // ZZZZZ use proper consume

            this->text_signaled = true;
        }
    }
}

void EditableTextTemp::layout() {
    // assuming top left of box is set

    this->box.x_max = this->box.x_min + CHARACTER_WIDTH * this->text.size() + 2 * PAD_X;
    this->box.y_max = this->box.y_min + (float) font.baseSize / 2 + 2 * PAD_Y;
}

void EditableTextTemp::draw() {
    ray::Rectangle rect = box_to_rectangle(this->box);
    DrawRectangleRec(rect, this->color);

    DrawTextEx(font, this->text.c_str(), ray::Vector2{this->box.x_min + PAD_X, this->box.y_min + PAD_Y}, (float) font.baseSize / 2, 0, ray::WHITE);
    if (this->selected) {
        float k = b(this->character_index);
        ray::DrawLine(k, this->box.y_min+1, k, this->box.y_max, ray::Color{ 255, 255, 255, 255 });
    }
}
