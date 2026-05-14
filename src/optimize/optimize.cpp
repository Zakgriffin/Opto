#include "optimize.h"

ObjectViewBuilder optimize_object_view_builder = ObjectViewBuilder{OPTIMIZE, "compile", optimize_create_simple, optimize_create_sub_object_views, optimize_destroy_sub_object_views};

struct OptimizeObjectViewContext {
    Box optimize_button_box;
};

void *optimize_create_simple() {
    auto optimize = new Optimize{.source = nullptr, .passes = nullptr, .stages = nullptr};
    object_to_type.insert({optimize, OPTIMIZE});
    return optimize;
}

void optimize_create_sub_object_views(ObjectView *optimize_view) {
    auto optimize_handle = (Optimize **) optimize_view->object_handle;
    auto optimize = *optimize_handle;

    auto context = new OptimizeObjectViewContext;
    optimize_view->context = context;

    optimize_view->internal_constraints.push_back(create_listener({&optimize_view->editable_text.box_sig}, new function<void(void)>([=]() {
        context->optimize_button_box.x_min = optimize_view->editable_text.box.x_min;
        context->optimize_button_box.x_max = optimize_view->editable_text.box.x_min + 30;
        context->optimize_button_box.y_min = optimize_view->editable_text.box.y_min - 30;
        context->optimize_button_box.y_max = optimize_view->editable_text.box.y_max - 30;
    })));

    generic_linear_under_create_views(optimize_view, &Optimize::source, &Optimize::passes);

    optimize_view->internal_constraints.push_back(create_listener({&input_listeners}, new function<void(void)>([=]() {
        auto mouse = ray::GetMousePosition();
        if (is_within_box(mouse, context->optimize_button_box)) {
            mouse_cursor = ray::MOUSE_CURSOR_POINTING_HAND;
            if (ray::IsMouseButtonPressed(0)) {
                llvm::Module *module = nullptr;

                auto stages = typed(VECTOR, new vector<void*>);

                if (object_to_type.at(optimize->source) == PROCEDURE) {
                    module = build_llvm(optimize->source);
                    stages->push_back(module);

                    std::vector<char> buffer;
                    serialize_ast(optimize->source, buffer);

                    cout << "BUFFER LEN: " << buffer.size() << endl;
                } else if (object_to_type.at(optimize->source) == LLVM_MODULE) {
                    module = (llvm::Module*) optimize->source;
                }

                if (!module) return;

                if (object_to_type.at(optimize->passes) != VECTOR) return;
                auto passes_temp = (vector<void*>*) optimize->passes;

                int offset = 0;
                for (auto pass_untyped : *passes_temp) {
                    auto pass = (function<void(llvm::Function&, llvm::FunctionAnalysisManager&)>*) pass_untyped;
                    if (object_to_type.at(pass) != LLVM_PASS) return;

                    auto clone = llvm::CloneModule(*module).release();
                    perform_pass(clone, *pass);
                    supplement_llvm_module(clone);

                    stages->push_back(clone);
                }

                for (auto stage : *stages) {
                    auto stages_root = new void*;
                    *stages_root = stage;
                    auto object_view = new_object_view(stages_root);

                    move_box_x(&object_view->editable_text.box, optimize_view->box.x_min + offset);
                    move_box_y(&object_view->editable_text.box, optimize_view->box.y_max + 30);
                    signal_update(&object_view->editable_text.box_sig);

                    offset += 120;
                }

                // optimize->stages = stages;
            }
        }
    })));

    optimize_view->internal_constraints.push_back(create_listener({&draw_visuals}, new function<void(void)>([=]() {
        DrawRectangleRec(box_to_rectangle(context->optimize_button_box), ray::ORANGE);
    })));
}

void optimize_destroy_sub_object_views(ObjectView *optimize_view) {
    generic_destroy_sub_object_views(optimize_view);
}