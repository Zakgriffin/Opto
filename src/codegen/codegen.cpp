#include "codegen.h"

// void traverse_over_scopes(void* flow, function<void(void*, ObjectType)> on_open, function<void(void*, ObjectType)> on_close) {
//     auto current = flow;
//     stack<void*> scopes;
//     stack<void*> scope_nexts;
//
//     auto type = object_to_type.at(current);
//     on_open(current, type);
//     if (type == NONE) {
//         if (scopes.empty()) return;
//
//         auto scope = scopes.top();
//         on_close(scope, object_to_type.at(scope));
//         scopes.pop();
//
//         current = scope_nexts.top();
//         scope_nexts.pop();
//     } else if (type == DO_THEN) {
//         auto do_then = (DoThen*) current;
//         current = do_then->next;
//     } else if (type == LOOP) {
//         auto loop = (Loop*) current;
//         current = loop->body;
//         scopes.push(loop);
//         scope_nexts.push(nullptr);
//     } else if (type == WHILE) {
//         auto while_ = (While*) current;
//         current = while_->then;
//         scopes.push(while_);
//         scope_nexts.push(while_->finally);
//     } else if (type == REPEAT) {
//         auto repeat = (Repeat*) current;
//         current = repeat->then;
//         scopes.push(repeat);
//         scope_nexts.push(repeat->finally);
//     }
// }

void traverse_over_scopes(void* current, function<void(DoThen*)> on_each, function<void(void*, ObjectType)> on_open, function<void(void*, ObjectType)> on_close) {
    ObjectType type;
    while (true) {
        type = object_to_type.at(current);
        if (type != DO_THEN) break;
        auto do_then = (DoThen*) current;
        on_each(do_then);
        current = do_then->next;
    }

    if (!current) return;

    on_open(current, type);

    switch (type) {
    case LOOP: {
        auto node = (Loop*) current;
        traverse_over_scopes(node->body, on_each, on_open, on_close);
        break;
    }
    case WHILE: {
        auto node = (While*) current;
        traverse_over_scopes(node->then, on_each, on_open, on_close);
        traverse_over_scopes(node->finally, on_each, on_open, on_close);
        break;
    }
    case REPEAT: {
        auto node = (Repeat*) current;
        traverse_over_scopes(node->then, on_each, on_open, on_close);
        traverse_over_scopes(node->finally, on_each, on_open, on_close);
        break;
    }
    default:
        abort();
        break;
    }

    on_close(current, type);
}

void* removed_scope_flow(void* flow) {
    void* current;
    void** current_handle = &current;
    auto result = &current;

    unordered_map<void*, void*> scoped_to_flat_jump;

    traverse_over_scopes(flow, [&](DoThen* each){
        append_do_then(&current_handle, each->effect);
    }, [&](void* opened, ObjectType type){
        if (type == LOOP) {
            scoped_to_flat_jump.insert({opened, current});
        } else if (type == REPEAT) {
            scoped_to_flat_jump.insert({opened, current});
        }
    }, [&](void* closed, ObjectType type){
        void* effect = nullptr;

        if (type == LOOP) {
            auto loop = (Loop*) closed;
            // effect = typed(JUMP, new Jump{.jump = scoped_to_flat_jump.at(loop)});
            effect = typed(JUMP, new Jump{.jump = nullptr});
        } else if (type == REPEAT) {
            auto repeat = (Repeat*) closed;
            effect = typed(CONDITIONAL_JUMP, new ConditionalJump{.condition = repeat->condition, .jump = nullptr});
        }

        append_do_then(&current_handle, effect);
    });
    return *result;
}

void traverse_over_flat(void* flow, function<void(DoThen*)> on_each) {
    auto current = flow;
    while (current != nullptr) {
        auto do_then = (DoThen*) current;
        on_each(do_then);
        current = do_then->next;
    }
}