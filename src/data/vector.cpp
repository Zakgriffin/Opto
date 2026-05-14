#include "object_view.h"
#include "vector.h"

ObjectViewBuilder vector_object_view_builder = ObjectViewBuilder{VECTOR, "vector", vector_create_simple, vector_create_sub_object_views, vector_destroy_sub_object_views};

void *vector_create_simple() {
    auto vector_ = new vector<void*>{};
    object_to_type.insert({vector_, VECTOR});
    return vector_;
}

void vector_create_sub_object_views(ObjectView *vector_view) {
    auto handle = (vector<void*>**) vector_view->object_handle;
    vector<void*>* obj = *handle;

    Box* prev_box = &vector_view->editable_text.box;
    auto prev_sig = &vector_view->editable_text.box_sig;

    for (auto & element : *obj) {
        auto current_view = new_object_view(&element);
        quick_layout_right(vector_view, current_view, prev_box, prev_sig, &current_view->editable_text.box, &current_view->editable_text.box_sig);

        prev_box = &current_view->box;
        prev_sig = &current_view->box_sig;
    }


    vector_view->editable_text.on_keys.push_back([=]() {
        if (key_pressed == ray::KEY_COMMA) {
            obj->push_back(nullptr);
            *vector_view->object_handle = obj;
            signal_update(&object_to_signal.at(vector_view->object_handle)->o);
            return true;
        }
        return false;
    });
}

void vector_destroy_sub_object_views(ObjectView *vector_view) {
    generic_destroy_sub_object_views(vector_view);
}