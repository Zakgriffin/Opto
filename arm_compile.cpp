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

void* removed_scope_flow(void* flow) {
    auto root = typed(DO_THEN, new DoThen{.effect = nullptr, .next = nullptr});
    auto c_generated = root;

    auto c = flow;
    while (1) {
        if (c == nullptr) {
            break;
        }

        auto type = object_to_type.at(c);
        if (type == DO_THEN) {
            auto do_then = (DoThen*) c;

            c_generated->effect = do_then->effect;

            auto next = typed(DO_THEN, new DoThen{.effect = nullptr, .next = nullptr});
            c_generated->next = next;

            c = do_then->next;
        } else if (type == WHILE) {
            auto while_ = (While*) c;

            // c_generated->effect = typed(CONDITIONAL_JUMP, new ConditionalJump{.condition = while_->condition, .jump = nullptr});;

        } else {
            cout << "BAD SCOPY" << endl;
            break;
        }

    }

    return root;
}