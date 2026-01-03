#include "arm_compile.h"

ObjectViewBuilder conditional_jump_object_view_builder = ObjectViewBuilder{CONDITIONAL_JUMP, "conditional-jump", conditional_jump_create_simple, conditional_jump_create_sub_object_views, conditional_jump_destroy_sub_object_views};
void *conditional_jump_create_simple() {
    auto conditional_jump = new ConditionalJump{.condition = nullptr, .jump = nullptr};
    object_to_type.insert({conditional_jump, CONDITIONAL_JUMP});
    return conditional_jump;
}
void conditional_jump_create_sub_object_views(ObjectView *conditional_jump_view) { generic_linear_create_views(conditional_jump_view, &ConditionalJump::condition, &ConditionalJump::jump); }
void conditional_jump_destroy_sub_object_views(ObjectView *conditional_jump_view) { generic_destroy_sub_object_views(conditional_jump_view); }

ObjectViewBuilder jump_object_view_builder = ObjectViewBuilder{JUMP, "jump", jump_create_simple, jump_create_sub_object_views, jump_destroy_sub_object_views};
void *jump_create_simple() {
    auto jump = new Jump{ .jump = nullptr};
    object_to_type.insert({jump, JUMP});
    return jump;
}
void jump_create_sub_object_views(ObjectView *jump_view) { generic_linear_create_views(jump_view, &Jump::jump); }
void jump_destroy_sub_object_views(ObjectView *jump_view) { generic_destroy_sub_object_views(jump_view); }

void append(void** root_handle, void* effect) {
    *root_handle = typed(DO_THEN, new DoThen{.effect = effect, .next = nullptr});
}

void traverse_over_scopes(void* flow, function<void(void*, ObjectType)> on_open, function<void(void*, ObjectType)> on_close) {
    auto current = flow;
    stack<void*> scopes;
    stack<void*> scope_nexts;

    while (1) {
        auto type = object_to_type.at(current);
        on_open(current, type);
        if (type == NONE) {
            if (scopes.empty()) break;

            auto scope = scopes.top();
            on_close(scope, object_to_type.at(scope));
            scopes.pop();

            current = scope_nexts.top();
            scope_nexts.pop();
        } else if (type == DO_THEN) {
            auto do_then = (DoThen*) current;
            current = do_then->next;
        } else if (type == LOOP) {
            auto loop = (Loop*) current;
            current = loop->body;
            scopes.push(loop);
            scope_nexts.push(nullptr);
        } else if (type == WHILE) {
            auto while_ = (While*) current;
            current = while_->then;
            scopes.push(while_);
            scope_nexts.push(while_->finally);
        } else if (type == REPEAT) {
            auto repeat = (Repeat*) current;
            current = repeat->then;
            scopes.push(repeat);
            scope_nexts.push(repeat->finally);
        }
    }
}

void* bungus_open(void* each, ObjectType type) {
    if (type == DO_THEN) {
        auto do_then = (DoThen*) each;
        return do_then->effect;
    }

    return nullptr;
}

void* bungus_close(void* each, ObjectType type) {
    cout<< type << endl;
    if (type == LOOP) {
        return typed(JUMP, new Jump{.jump = nullptr});
    }
    if (type == REPEAT) {
        auto repeat = (Repeat*) each;
        return typed(CONDITIONAL_JUMP, new ConditionalJump{.condition = repeat->condition, .jump = nullptr});
    }

    return nullptr;
}

void* removed_scope_flow(void* flow) {
    void* current;
    void** current_handle = &current;
    auto result = &current;

    traverse_over_scopes(flow, [&](void* each, ObjectType type){
        void* effect = bungus_open(each, type);
        if (effect == nullptr) return;

        auto k = typed(DO_THEN, new DoThen{.effect = effect, .next = nullptr});
        *current_handle = k;
        current_handle = &k->next;
    },[&](void* each, ObjectType type){
        void* effect = bungus_close(each, type);
        if (effect == nullptr) return;

        auto k = typed(DO_THEN, new DoThen{.effect = effect, .next = nullptr});
        *current_handle = k;
        current_handle = &k->next;
    });
    return *result;
}