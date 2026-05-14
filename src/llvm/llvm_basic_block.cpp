#include "object_view.h"
#include "llvm_headers.h"
#include "llvm_basic_block.h"

ObjectViewBuilder llvm_basic_block_object_view_builder = ObjectViewBuilder{LLVM_BASIC_BLOCK, "basic_block", llvm_basic_block_create_simple, llvm_basic_block_create_sub_object_views, llvm_basic_block_destroy_sub_object_views};

void *llvm_basic_block_create_simple() {
    auto llvm_basic_block = nullptr; // ZZZZ nope
    object_to_type.insert({llvm_basic_block, LLVM_BASIC_BLOCK});
    return llvm_basic_block;
}

void llvm_basic_block_create_sub_object_views(ObjectView *llvm_basic_block_view) {
    auto handle = (llvm::BasicBlock**) llvm_basic_block_view->object_handle;
    auto obj = *handle;

    Box* prev_box = &llvm_basic_block_view->editable_text.box;
    auto prev_sig = &llvm_basic_block_view->editable_text.box_sig;

    for (auto &instruction_ref : *obj) {
        auto instruction = &instruction_ref;
        auto instruction_aux = new void*;
        *instruction_aux = instruction;

        auto current_view = new_object_view(instruction_aux);
        quick_layout_under(llvm_basic_block_view, current_view, prev_box, prev_sig, &current_view->editable_text.box, &current_view->editable_text.box_sig);

        prev_box = &current_view->box;
        prev_sig = &current_view->box_sig;
    }
}

void llvm_basic_block_destroy_sub_object_views(ObjectView *llvm_basic_block_view) {
    generic_destroy_sub_object_views(llvm_basic_block_view);
}