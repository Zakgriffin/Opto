#include "data.h"
#include "expressions.h"
#include "object_view.h"
#include "procedures.h"
#include "run.h"
#include "type.h"

unordered_map<Variable*, void**> variable_to_handle;

int evaluate_num_expression(void* expression) {
    auto type = object_to_type.at(expression);
    if (type == ADD) {
        auto add = (Add*) expression;
        return evaluate_num_expression(add->addend) + evaluate_num_expression(add->augend);
    }
    if (type == SUB) {
        auto sub = (Sub*) expression;
        return evaluate_num_expression(sub->minuend) - evaluate_num_expression(sub->subtrahend);
    }
    if (type == AND) {
        auto and_ = (And*) expression;
        return evaluate_num_expression(and_->conjunct_1) && evaluate_num_expression(and_->conjunct_2);
    }
    if (type == INTEGER) {
        auto integer = (int*) expression;
        return *integer;
    }
    if (type == GREATER_THAN) {
        auto greater_than = (GreaterThan*) expression;
        return evaluate_num_expression(greater_than->left) > evaluate_num_expression(greater_than->right);
    }
    if (type == LESS_THAN) {
        auto less_than = (LessThan*) expression;
        return evaluate_num_expression(less_than->left) < evaluate_num_expression(less_than->right);
    }
    if (type == GREATER_THAN_OR_EQUAL) {
        auto greater_than_or_equal = (GreaterThanOrEqual*) expression;
        return evaluate_num_expression(greater_than_or_equal->left) >= evaluate_num_expression(greater_than_or_equal->right);
    }
    if (type == VARIABLE) {
        auto variable = (Variable*) expression;
        auto handle = variable_to_handle.at(variable);
        return evaluate_num_expression(*handle);
    }
    if (type == INDEX) {
        auto index = (Index*) expression;
        auto v = (vector<void*>*) evaluate_expression(index->array);
        auto at = evaluate_num_expression(index->at);
        return *(int*)v->at(at);
    }

    cout << "bad expresion" << endl;

    return -1;
}

void* evaluate_expression(void* expression) {
    auto type = object_to_type.at(expression);
    if (type == VARIABLE) {
        auto variable = (Variable*) expression;
        auto handle = variable_to_handle.at(variable);
        return *handle;
    }
    if (type == VECTOR) {
        return expression; // ZZZZ maybe copy and such
    }

    auto evaluated = evaluate_num_expression(expression);
    auto i = (int*) integer_create_simple();
    *i = evaluated;
    return i;
}

void onto_next(Run* run, void* next) {
    auto next_type = object_to_type.at(next);
    if (next_type == NONE) {
        if (run->scope_stack.empty()) {
            onto_next(run, run->start); // restart ZZZZ move out!
        } else {
            auto callback = run->scope_stack.top();
            run->scope_stack.pop();
            callback();
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
            if (evaluate_num_expression(while_->condition)) {
                onto_next(run, while_->then);
            } else {
                run->scope_stack.pop();
                onto_next(run, while_->finally);
            }
        };
        run->scope_stack.push(while_end_scope);
        while_end_scope();
    } else if (next_type == REPEAT) {
        auto repeat = (Repeat*) next;
        auto repeat_end_scope = [=]{
            if (evaluate_num_expression(repeat->condition)) {
                onto_next(run, repeat->then);
            } else {
                run->scope_stack.pop();
                onto_next(run, repeat->finally);
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
    if (type == VARIABLE) {
        auto variable = (Variable*) effect;

        if (!variable_to_handle.contains(variable)) {
            auto recent_root = new void *;
            *recent_root = nullptr;
            auto object_view = new_object_view(recent_root);
            variable_to_handle.insert({variable, object_view->object_handle});

            if (object_to_view.contains(run->current)) {
                auto view = object_to_view.at(run->current);
                move_box_x(&object_view->editable_text.box, view->box.x_max + 100);
                move_box_y(&object_view->editable_text.box, view->box.y_min);
                signal_update(&object_view->editable_text.box_sig);
            }
        }
    } else if (type == ASSIGN) {
        auto assign = (Assign*) effect;
        auto grantee = assign->grantee;
        auto grantor = assign->grantor;

        auto grantee_type = object_to_type.at(grantee);
        if (grantee_type == INDEX) {
            auto index = (Index*) grantee;
            auto v = (vector<void*>*) evaluate_expression(index->array);
            auto at = evaluate_num_expression(index->at);

            auto grantor_evaluated = evaluate_expression(grantor);
            v->at(at) = grantor_evaluated;
        } else {
            auto grantee_var = (Variable*) grantee;

            // ZZZZ this is ugly
            if (run->vars.contains(grantee_var)) {
                cout << "mapped variable" << endl;
                grantee_var = run->vars.at(grantee_var);
            }

            auto object_handle = variable_to_handle.at(grantee_var);
            auto grantor_evaluated = evaluate_expression(grantor);
            *object_handle = grantor_evaluated;
            signal_update(&object_to_signal.at(object_handle)->o);
        }
    } else if (type == CALL) {
        cout << "CALL" << endl;
        auto call = (Call*) effect;
        if (object_to_type.at(call->procedure) != PROCEDURE) {
            cout << "not a procedure" << endl;
            return;
        }
        auto procedure = (Procedure*) call->procedure;
        auto return_instruction = run->current->next;

        run->vars.insert({(Variable*) procedure->param, (Variable*) call->param});

        auto call_end_scope = [=]{
            run->vars.erase((Variable*) procedure->param);
            onto_next(run, return_instruction);
        };

        run->scope_stack.push(call_end_scope);
        onto_next(run, procedure->body);
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
    Box type_check_button_box;
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

    run_view->internal_constraints.push_back(create_listener({&run_view->editable_text.box_sig}, new function<void(void)>([=]() {
        context->type_check_button_box.x_min = run_view->editable_text.box.x_min;
        context->type_check_button_box.x_max = run_view->editable_text.box.x_max;
        context->type_check_button_box.y_min = run_view->editable_text.box.y_min - 60;
        context->type_check_button_box.y_max = run_view->editable_text.box.y_max - 60;
    })));

    auto start_view = new_object_view((void **) &run->start);
    quick_layout_right(run_view, start_view, &run_view->editable_text.box, &run_view->editable_text.box_sig, &start_view->editable_text.box, &start_view->editable_text.box_sig);

    run_view->internal_constraints.push_back(create_listener({&input_listeners}, new function<void(void)>([=]() {
        auto mouse = ray::GetMousePosition();
        if (is_within_box(mouse, context->step_button_box)) {
            mouse_cursor = ray::MOUSE_CURSOR_POINTING_HAND;
            if (ray::IsMouseButtonPressed(0)) {
                auto x = (DoThen **) start_view->object_handle;
                auto start = *x;
                if (start == nullptr) return;

                run->start = start;
                click_step(run);
            }
        } else if (is_within_box(mouse, context->type_check_button_box)) {
            mouse_cursor = ray::MOUSE_CURSOR_POINTING_HAND;
            if (ray::IsMouseButtonPressed(0)) {
                unordered_map<void*, ObjectType> env;
                type_check(run->start, &env);
            }
        }
    })));

    run_view->internal_constraints.push_back(create_listener({&draw_visuals}, new function<void(void)>([=]() {
        DrawRectangleRec(box_to_rectangle(context->step_button_box), (ray::Color){ 230, 41, 55, 255 });
        DrawRectangleRec(box_to_rectangle(context->type_check_button_box), (ray::Color){ 0, 0, 255, 255 });

        if(!object_to_view.contains(run->current)) return;

        auto current_view = object_to_view.at(run->current);
        ray::DrawLine(current_view->box.x_min, current_view->box.y_min, current_view->box.x_max, current_view->box.y_min, (ray::Color){ 230, 41, 55, 255 });

        // if (object_to_view->next_box != nullptr) {
        //     DrawRectangleRec(box_to_rectangle(object_to_view->next_box), Color{255,0,0,50});
        // }
    })));

}

void run_destroy_sub_object_views(ObjectView *run_view) {
    delete (RunObjectViewContext*) run_view->context;
    generic_destroy_sub_object_views(run_view);
}