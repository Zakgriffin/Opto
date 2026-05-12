#include "llvm_function.h"

ObjectViewBuilder llvm_function_object_view_builder = ObjectViewBuilder{LLVM_FUNCTION, "function", llvm_function_create_simple, llvm_function_create_sub_object_views, llvm_function_destroy_sub_object_views};

void *llvm_function_create_simple() {
    auto llvm_function = nullptr; // ZZZZ garbo

    object_to_type.insert({llvm_function, LLVM_FUNCTION});
    return llvm_function;
}

void llvm_function_create_sub_object_views(ObjectView *llvm_function_view) {
    auto handle = (llvm::Function**) llvm_function_view->object_handle;
    auto obj = *handle;

    Box* prev_box = &llvm_function_view->editable_text.box;
    auto prev_sig = &llvm_function_view->editable_text.box_sig;

    for (auto &basic_block_ref : *obj) {
        auto basic_block = &basic_block_ref;
        auto basic_block_aux = new void*;
        *basic_block_aux = basic_block;

        auto current_view = new_object_view(basic_block_aux);
        quick_layout_under(llvm_function_view, current_view, prev_box, prev_sig, &current_view->editable_text.box, &current_view->editable_text.box_sig);

        prev_box = &current_view->box;
        prev_sig = &current_view->box_sig;
    }
}

void llvm_function_destroy_sub_object_views(ObjectView *llvm_function_view) {
    generic_destroy_sub_object_views(llvm_function_view);
}