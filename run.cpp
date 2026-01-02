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

void click_step(Run *run) {
    if (run->start == nullptr) return;

    if (run->current == nullptr) {
        if (run->scope_stack.empty()) {
            run->current = run->start;
        } else {
            run->current = run->scope_stack.top();
            run->scope_stack.pop();
        }
    }

    auto control_type = object_to_type.at(run->current);
    if (control_type == DO_THEN) {
        auto do_then = (DoThen*) run->current;
        auto effect = do_then->effect;

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
        } else {
            cout << "ZZZZ dont know dis effect" << endl;
        }

        run->current = do_then->next;
    } else if (control_type == IF)
    {
        auto if_ = (If*) run->current;
        if (evaluate_expression(if_->condition)) {
            run->current = if_->then;
            run->scope_stack.push(if_->finally);
        } else {
            run->current = if_->finally;
        }
    } else if (control_type == LOOP) {
        auto loop = (Loop*) run->current;
        run->current = loop->body;
        run->scope_stack.push(loop);
    } else if (control_type == WHILE) {
        auto while_ = (While*) run->current;
        if (evaluate_expression(while_->condition)) {
            run->current = while_->then;
            run->scope_stack.push(while_);
        } else {
            run->current = while_->finally;
        }
    } else if (control_type == REPEAT) {
        // auto repeat = (Repeat*) run->current;
        // if (evaluate_expression(repeat->condition)) {
        //     run->current = repeat->then;
        //     run->scope_stack.push(repeat);
        // } else {
        //     run->current = repeat->finally;
        // }
    }
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
        // auto obj = *current_view->object_handle;
        // auto type = object_to_type.at(obj);
        // if (type == DO_THEN) {
        //     auto do_then = (DoThen*) obj;
        // DrawLine(current_view->box.x_min, current_view->box.y_min, current_view->box.x_max, current_view->box.y_min, RED);
        //
        // } else if (type == WHILE) {
        //
        // }
        DrawLine(current_view->box.x_min, current_view->box.y_min, current_view->box.x_max, current_view->box.y_min, RED);
    })));


}

void run_destroy_sub_object_views(ObjectView *run_view) {
    delete (RunObjectViewContext*) run_view->context;
    generic_destroy_sub_object_views(run_view);
}