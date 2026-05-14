#include "object_view.h"
#include "compile.h"
#include "llvm_codegen.h"

ObjectViewBuilder compile_object_view_builder = ObjectViewBuilder{COMPILE, "compile", compile_create_simple, compile_create_sub_object_views, compile_destroy_sub_object_views};

struct CompileObjectViewContext {
    Box compile_button_box;
};

void *compile_create_simple() {
    auto compile = new Compile{.source = nullptr, .passes = nullptr, .stages = nullptr};
    object_to_type.insert({compile, COMPILE});
    return compile;
}

void compile_create_sub_object_views(ObjectView *compile_view) {
    auto compile_handle = (Compile **) compile_view->object_handle;
    auto compile = *compile_handle;

    auto context = new CompileObjectViewContext;
    compile_view->context = context;

    compile_view->internal_constraints.push_back(create_listener({&compile_view->editable_text.box_sig}, new function<void(void)>([=]() {
        context->compile_button_box.x_min = compile_view->editable_text.box.x_min;
        context->compile_button_box.x_max = compile_view->editable_text.box.x_min + 30;
        context->compile_button_box.y_min = compile_view->editable_text.box.y_min - 30;
        context->compile_button_box.y_max = compile_view->editable_text.box.y_max - 30;
    })));

    generic_linear_under_create_views(compile_view, &Compile::source, &Compile::passes);

    compile_view->internal_constraints.push_back(create_listener({&input_listeners}, new function<void(void)>([=]() {
        auto mouse = ray::GetMousePosition();
        if (is_within_box(mouse, context->compile_button_box)) {
            mouse_cursor = ray::MOUSE_CURSOR_POINTING_HAND;
            if (ray::IsMouseButtonPressed(0)) {
                llvm::Module *module = nullptr;

                auto stages = typed(VECTOR, new vector<void*>);

                if (object_to_type.at(compile->source) == PROCEDURE) {
                    module = build_llvm(compile->source);
                    stages->push_back(module);

                    std::vector<char> buffer;
                    serialize_ast(compile->source, buffer);

                    cout << "BUFFER LEN: " << buffer.size() << endl;
                } else if (object_to_type.at(compile->source) == LLVM_MODULE) {
                    module = (llvm::Module*) compile->source;
                }

                if (!module) return;

                if (object_to_type.at(compile->passes) != VECTOR) return;
                auto passes_temp = (vector<void*>*) compile->passes;

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

                    move_box_x(&object_view->editable_text.box, compile_view->box.x_min + offset);
                    move_box_y(&object_view->editable_text.box, compile_view->box.y_max + 30);
                    signal_update(&object_view->editable_text.box_sig);

                    offset += 120;
                }

                // compile->stages = stages;
            }
        }
    })));

    compile_view->internal_constraints.push_back(create_listener({&draw_visuals}, new function<void(void)>([=]() {
        DrawRectangleRec(box_to_rectangle(context->compile_button_box), ray::ORANGE);
    })));
}

void compile_destroy_sub_object_views(ObjectView *compile_view) {
    generic_destroy_sub_object_views(compile_view);
}