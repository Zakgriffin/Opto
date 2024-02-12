#ifndef OPTO_DO_THEN_H
#define OPTO_DO_THEN_H

#include <vector>
#include "do_then.h"
#include "lookup_box.h"
#include "has_bounding_box.h"
#include "list.h"

using namespace std;

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

    Rectangle *get_bounding_box() override {
        return &lookup_box->rect;
    }
};

class DoThenNextView {
public:
    virtual ~DoThenNextView();
};

class DoThenNextAsNullView : public DoThenNextView {
public:
    LookupBox *lookup_box;

    explicit DoThenNextAsNullView(DoThenView *parent);

    ~DoThenNextAsNullView() override;
};

class DoThenNextAsDoThenView : public DoThenNextView {
public:
    DoThenView *do_then_view;

    explicit DoThenNextAsDoThenView(DoThenView *parent);

    ~DoThenNextAsDoThenView() override;
};

class DoThenNextAsListView : public DoThenNextView {
public:
    ListView *list_view;

    explicit DoThenNextAsListView(DoThenView *parent);

    ~DoThenNextAsListView() override;
};

class DoThenEffectView {
public:
    virtual ~DoThenEffectView();
};

class DoThenEffectAsNullView : public DoThenEffectView {
public:
    LookupBox *lookup_box;

    explicit DoThenEffectAsNullView(DoThenView *parent);

    ~DoThenEffectAsNullView() override;
};

class DoThenEffectAsAnyView : public DoThenEffectView {
public:
    explicit DoThenEffectAsAnyView(DoThenView *parent);

    ~DoThenEffectAsAnyView() override;
};

#endif //OPTO_DO_THEN_H
