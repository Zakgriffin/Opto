#include "run.h"

int evaluate_expression(void* expression) {
    auto type = object_to_type.at(expression);
    if (type == ADD) {
        auto add = (Add*) expression;
        return evaluate_expression(add->addend) + evaluate_expression(add->augend);
    }
    if (type == INTEGER) {
        auto integer = (int*) expression;
        return *integer;
    }
    if (type == GREATER_THAN) {
        auto greater_than = (GreaterThan*) expression;
        return evaluate_expression(greater_than->left) > evaluate_expression(greater_than->right);
    }
    if (type == DECLARE) {
        auto declare = (Declare*) expression;
        auto handle = name_to_object_handle.at(*declare->name);
        return evaluate_expression(*handle);
    }

    return 0;
}

void onto_next(Run* run, void* next) {
    auto next_type = object_to_type.at(next);
    if (next_type == NONE) {
        if (run->scope_stack.empty()) {
            onto_next(run, run->start); // restart ZZZZ move out!
        } else {
            run->scope_stack.top()();
        }
    } else if (next_type == DO_THEN) {
        auto do_then = (DoThen*) next;
        run->current = do_then;
    } else if (next_type == LOOP) {
        auto loop = (Loop*) next;
        auto loop_end_scope = [=]{
            onto_next(run, loop->body);
        };
        run->scope_stack.push(loop_end_scope);
        loop_end_scope();
    } else if (next_type == WHILE) {
        auto while_ = (While*) next;
        auto while_end_scope = [=]{
            if (evaluate_expression(while_->condition)) {
                onto_next(run, while_->then);
            } else {
                onto_next(run, while_->finally);
                run->scope_stack.pop();
            }
        };
        run->scope_stack.push(while_end_scope);
        while_end_scope();
    } else if (next_type == REPEAT) {
        auto repeat = (Repeat*) next;
        auto repeat_end_scope = [=]{
            if (evaluate_expression(repeat->condition)) {
                onto_next(run, repeat->then);
            } else {
                onto_next(run, repeat->finally);
                run->scope_stack.pop();
            }
        };
        run->scope_stack.push(repeat_end_scope);
        onto_next(run, repeat->then);
    }
}

void click_step(Run* run) {
    if (run->current == nullptr) {
        onto_next(run, run->start);
    }

    auto effect = run->current->effect;

    auto type = object_to_type.at(effect);
    if (type == DECLARE) {
        auto declare = (Declare*) effect;

        if (!name_to_object_handle.contains(*declare->name)) {
            auto recent_root = new void *;
            *recent_root = nullptr;
            auto object_view = new_object_view(recent_root);
            name_to_object_handle.insert({*declare->name, object_view->object_handle});

            auto view = object_to_view.at(run->current);
            move_box_x(&object_view->editable_text.box, view->box.x_max + 100);
            move_box_y(&object_view->editable_text.box, view->box.y_min);
            signal_update(&object_view->editable_text.box_sig);
        }
    } else if (type == ASSIGN) {
        auto assign = (Assign*) effect;
        auto grantee = (Declare*) assign->grantee;
        auto grantor = (void*) assign->grantor;

        auto object_handle = name_to_object_handle.at(*grantee->name);
        auto assigned = evaluate_expression(grantor);
        *object_handle = integer_create_simple();
        *(int*) *object_handle = assigned;
        signal_update(&object_to_signal.at(object_handle)->o);
    } else if (type == NONE) {
        cout << "ZZZZ no effect" << endl;
    } else {
        cout << "ZZZZ unknown effect" << endl;
    }

    auto next = run->current->next;
    onto_next(run, next);
}

ObjectViewBuilder run_object_view_builder = ObjectViewBuilder{RUN, "run", run_create_simple, run_create_sub_object_views, run_destroy_sub_object_views};

void *run_create_simple() {
    auto run = new Run{.start = nullptr, .current = nullptr};
    object_to_type.insert({run, RUN});
    return run;
}

struct RunObjectViewContext {
    Box step_button_box;
};

void run_create_sub_object_views(ObjectView *run_view) {
    auto run_handle = (Run **) run_view->object_handle;
    auto run = *run_handle;

    auto context = new RunObjectViewContext;
    run_view->context = context;
    run_view->internal_constraints.push_back(create_listener({&run_view->editable_text.box_sig}, new function<void(void)>([=]() {
        context->step_button_box.x_min = run_view->editable_text.box.x_min;
        context->step_button_box.x_max = run_view->editable_text.box.x_max;
        context->step_button_box.y_min = run_view->editable_text.box.y_min - 30;
        context->step_button_box.y_max = run_view->editable_text.box.y_max - 30;
    })));


    auto start_view = new_object_view((void **) &run->start);
    quick_layout_right(run_view, start_view, &run_view->editable_text.box, &run_view->editable_text.box_sig, &start_view->editable_text.box, &start_view->editable_text.box_sig);

    run_view->internal_constraints.push_back(create_listener({&input_listeners}, new function<void(void)>([=]() {
        auto mouse = GetMousePosition();
        if (is_within_box(mouse, context->step_button_box)) {
            mouse_cursor = MOUSE_CURSOR_POINTING_HAND;
            if (IsMouseButtonPressed(0)) {
                auto x = (DoThen **) start_view->object_handle;
                auto start = *x;
                if (start == nullptr) return;

                run->start = start;
                click_step(run);
            }
        }
    })));

    run_view->internal_constraints.push_back(create_listener({&draw_visuals}, new function<void(void)>([=]() {
        DrawRectangleRec(box_to_rectangle(context->step_button_box), RED);

        if(!object_to_view.contains(run->current)) return;

        auto current_view = object_to_view.at(run->current);
        DrawLine(current_view->box.x_min, current_view->box.y_min, current_view->box.x_max, current_view->box.y_min, RED);

        // if (object_to_view->next_box != nullptr) {
        //     DrawRectangleRec(box_to_rectangle(object_to_view->next_box), Color{255,0,0,50});
        // }
    })));


}

void run_destroy_sub_object_views(ObjectView *run_view) {
    delete (RunObjectViewContext*) run_view->context;
    generic_destroy_sub_object_views(run_view);
}