#include "llvm_headers.h"
#include "llvm_module.h"
#include "object_view.h"

ObjectViewBuilder llvm_module_object_view_builder = ObjectViewBuilder{LLVM_MODULE, "module", llvm_module_create_simple, llvm_module_create_sub_object_views, llvm_module_destroy_sub_object_views};

void *llvm_module_create_simple() {
    auto context = new llvm::LLVMContext;
    auto llvm_module = new llvm::Module("module", *context);
    object_to_type.insert({llvm_module, LLVM_MODULE});
    return llvm_module;
}

void llvm_module_create_sub_object_views(ObjectView *llvm_module_view) {
    auto handle = (llvm::Module**) llvm_module_view->object_handle;
    auto obj = *handle;

    Box* prev_box = &llvm_module_view->editable_text.box;
    auto prev_sig = &llvm_module_view->editable_text.box_sig;

    for (auto &function_ref : obj->getFunctionList()) {
        auto function = &function_ref;
        auto function_aux = new void*;
        *function_aux = function;

        auto current_view = new_object_view(function_aux);
        quick_layout_under(llvm_module_view, current_view, prev_box, prev_sig, &current_view->editable_text.box, &current_view->editable_text.box_sig);

        prev_box = &current_view->box;
        prev_sig = &current_view->box_sig;
    }
}

void llvm_module_destroy_sub_object_views(ObjectView *llvm_module_view) {
    generic_destroy_sub_object_views(llvm_module_view);
}