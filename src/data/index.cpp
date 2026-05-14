#include "object_view.h"
#include "index.h"

ObjectViewBuilder index_object_view_builder = ObjectViewBuilder{INDEX, "[", index_create_simple, index_create_sub_object_views, index_destroy_sub_object_views};

void *index_create_simple() {
    auto index = new Index{.array = nullptr, .at = nullptr};
    object_to_type.insert({index, INDEX});
    return index;
}

void index_create_sub_object_views(ObjectView *index_view) {
    generic_linear_create_views(index_view, &Index::array, &Index::at);
}

void index_destroy_sub_object_views(ObjectView *index_view) {
    generic_destroy_sub_object_views(index_view);
}