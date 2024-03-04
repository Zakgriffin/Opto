#ifndef OPTO_DO_THEN_H
#define OPTO_DO_THEN_H

#include <vector>
#include "do_then.h"
#include "lookup_box.h"
#include "has_bounding_box.h"
#include "list.h"

struct DoThen {
    void *effect;
    DoThen *next;
};

// i hate C++
class DoThenEffectView;

class DoThenNextView;

struct DoThenView : HasBoundingBox {
    DoThen *do_then;
    LookupBox *lookup_box; // on_lookup should be assigned by creator right after construction

    DoThenEffectView *effect_view;
    DoThenNextView *next_view;

    Fn *effect_datum_listener;
    Fn *next_datum_listener;

    explicit DoThenView(DoThen *do_then_in);

    ~DoThenView();

    Rectangle bounding_box;
    Rectangle *get_bounding_box() override {
        // TODO constrain bounding box
        return &bounding_box;
    }
};

class DoThenNextView : public HasBoundingBox{
public:
    virtual ~DoThenNextView() = default;
};

class DoThenNextAsNullView : public DoThenNextView {
public:
    LookupBox *lookup_box;

    explicit DoThenNextAsNullView(DoThenView *parent);

    Rectangle * get_bounding_box() override {
        return lookup_box->get_bounding_box();
    }

    ~DoThenNextAsNullView() override;
};

class DoThenNextAsDoThenView : public DoThenNextView {
public:
    DoThenView *do_then_view;

    explicit DoThenNextAsDoThenView(DoThenView *parent);

    Rectangle * get_bounding_box() override {
        return do_then_view->get_bounding_box();
    }

    ~DoThenNextAsDoThenView() override;
};

class DoThenNextAsListView : public DoThenNextView {
public:
    ListView *list_view;

    explicit DoThenNextAsListView(DoThenView *parent);

    ~DoThenNextAsListView() override;
};

class DoThenEffectView : public HasBoundingBox {
public:
    virtual ~DoThenEffectView() = default;
};

class DoThenEffectAsNullView : public DoThenEffectView {
public:
    LookupBox *lookup_box;

    explicit DoThenEffectAsNullView(DoThenView *parent);

    Rectangle * get_bounding_box() override {
        return lookup_box->get_bounding_box();
    }

    ~DoThenEffectAsNullView() override;
};

class DoThenEffectAsAnyView : public DoThenEffectView {
public:
    explicit DoThenEffectAsAnyView(DoThenView *parent);

    Rectangle * get_bounding_box() override {
        return nullptr;
    }

    ~DoThenEffectAsAnyView() override;
};

#endif //OPTO_DO_THEN_H
