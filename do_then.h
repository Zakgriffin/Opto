#ifndef OPTO_DO_THEN_H
#define OPTO_DO_THEN_H

#include <vector>
#include "do_then.h"
#include "lookup_box.h"

using namespace std;

struct DoThen {
    void *effect;
    DoThen *next;
};

// i hate C++
class DoThenEffectView;

class DoThenNextView;

struct DoThenView {
    DoThen *do_then;
    LookupBox *lookup_box; // on_lookup should be assigned by creator right after construction

    DoThenEffectView *effect_view;
    DoThenNextView *next_view;

    explicit DoThenView(DoThen *do_then_in);

    ~DoThenView();
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
