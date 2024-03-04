#ifndef OPTO_LIST_H
#define OPTO_LIST_H

#include "has_bounding_box.h"
#include "unknown.h"
#include "do_then.h"

struct ListItemView;

struct DoThenView;

class ListView {
public:
    vector<void *> *list;

    vector<ListItemView *> *item_views; // on_lookup should be assigned by creator right after construction

    Fn *list_datum_listener;

    Rectangle rect{}; // top and left bound externally, width and height bound internally

    explicit ListView(vector<void *> *);

    ~ListView();
};

struct ListItemView : HasBoundingBox {
public:
    virtual ~ListItemView();
};

class ListItemAsUnknownView : public ListItemView {
public:
    UnknownView *unknown_view;

    explicit ListItemAsUnknownView(ListView *parent, int i);

    ~ListItemAsUnknownView() override;

    Rectangle *get_bounding_box() override {
        return unknown_view->get_bounding_box();
    }
};

class ListItemAsDoThenView : public ListItemView {
public:
    DoThenView *do_then_view;

    explicit ListItemAsDoThenView(ListView *parent, int i);

    ~ListItemAsDoThenView() override;

    Rectangle *get_bounding_box() override {
        // this language is actually dog water
        // need cast because forward declaration can't inherit
        return ((HasBoundingBox *) do_then_view)->get_bounding_box();
    }
};

#endif //OPTO_LIST_H
