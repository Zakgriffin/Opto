#include "constraint.h"

#include <vector>
#include <unordered_map>
#include "matrix_math.h"
#include "priority_queue.h"
#include "globals.h"

#include <string>

using namespace std;

// variables

bool compare_binding_info(BindingInfo *left, BindingInfo *right) {
    return left->priority > right->priority;
}

unordered_map<void *, char> var_to_name;

unordered_map<void *, vector<F *> *> listeners;
unordered_map<void *, BindingInfo *> bindings;
PriorityQueue<BindingInfo *, compare_binding_info> bindings_to_update;

void debug_list_constraint() {
    int listeners_size = 0;
    for (auto x: listeners) {
        auto p = x.second;
        if(!p) continue; // TODO this should not be necessary
        listeners_size += (int) p->size();
    }
    printf("listeners: %d\n", listeners_size);

    printf("bindings: %d\n", (int) bindings.size());
}

// structs

bool operator<(const BindingInfo &left, const BindingInfo &right) {
    return left.priority < right.priority;
}

// functions

void update_all_tracked() {
//    if(bindings_to_update.is_empty()) {
//        printf("probably overusing update_all_tracked");
//        abort();
//    }

    while (!bindings_to_update.is_empty()) {
        BindingInfo *binding_info = bindings_to_update.pop();

        binding_info->update_binding();
        auto bound_data_listeners = listeners[binding_info->bound_data];
        if(bound_data_listeners) {
            for (auto x: *bound_data_listeners) {
                x->f();
            }
        }
    }
}

void test1() {
    // A = free
    // B = A + 10
    // C = G + 10
    // D = free
    // E = D + F + 10
    // F = D + 10
    // G = B + H + 10
    // H = free

    auto *A = new float;
    auto *B = new float;
    auto *C = new float;
    auto *D = new float;
    auto *E = new float;
    auto *F = new float;
    auto *G = new float;
    auto *H = new float;

    var_to_name[A] = 'A';
    var_to_name[B] = 'B';
    var_to_name[C] = 'C';
    var_to_name[D] = 'D';
    var_to_name[E] = 'E';
    var_to_name[F] = 'F';
    var_to_name[G] = 'G';
    var_to_name[H] = 'H';

    int stack_fill_1[1000];
    for (int &i: stack_fill_1) {
        i = 69;
    }

    int stack_fill_2[1000];
    for (int &i: stack_fill_2) {
        i = 69;
    }

    CleanupContext c;

    // 1
    c.make_binding(B, {A}, [=]() { *B = *A + 10; });
    c.make_binding(F, {D}, [=]() { *F = *D + 10; });
//    // 2
    c.make_binding(G, {B, H}, [=]() { *G = *B + *H + 10; });
//    // 3
    c.make_binding(E, {G, D, F}, [=]() { *E = *G + *D + *F + 10; });
    c.make_binding(C, {G}, [=]() { *C = *G + 10; });

    int stack_fill_3[1000];
    for (int &i: stack_fill_3) {
        i = 69;
    }

    update_listenable(A, 2.f);
    update_listenable(D, 3.f);
    update_listenable(H, 7.f);

    int stack_fill_4[1000];
    for (int &i: stack_fill_4) {
        i = 69;
    }

    update_all_tracked();

    debug_list_constraint();

//    printf("A: %f should be %f\n", *A, 2.);
//    printf("B: %f should be %f\n", *B, 12.);
//    printf("C: %f should be %f\n", *C, 39.);
//    printf("D: %f should be %f\n", *D, 3.);
//    printf("E: %f should be %f\n", *E, 55.);
//    printf("F: %f should be %f\n", *F, 13.);
//    printf("G: %f should be %f\n", *G, 29.);
//    printf("H: %f should be %f\n", *H, 7.);
//
//    int stack_fill_5[1000];
//    for (int &i: stack_fill_5) {
//        i = 0;
//    }
//
//    c.destroy_all();







//    destroy_binding(C);
//
//    update_listenable(A, 100.f);
//    update_all_tracked();
//
//    printf("\n");
//    printf("A: %f should be %f\n", *A, 100.);
//    printf("B: %f should be %f\n", *B, 110.);
//    printf("C: %f should be %f\n", *C, 39.);
//    printf("D: %f should be %f\n", *D, 3.);
//    printf("E: %f should be %f\n", *E, 153.);
//    printf("F: %f should be %f\n", *F, 13.);
//    printf("G: %f should be %f\n", *G, 127.);
//    printf("H: %f should be %f\n", *H, 7.);
}

struct S {
    float a;
};

bool compo(int left, int right) {
    return left < right;
}

void constraint_testing() {
    test1();
}
//
//struct RectangleInfo {
//    float left;
//    float right;
//    float top;
//    float bottom;
//
//    float width;
//    float height;
//};

//void setup_rectangles() {
//    auto *E = (RectangleInfo *) malloc(sizeof(RectangleInfo));
//
//    ConstraintSystem system;
//
//    vector<CoPair> co_pairs_width;
//    co_pairs_width.push_back({1, &E->right});
//    co_pairs_width.push_back({-1, &E->left});
//    system.add_constraint({.co_pairs = co_pairs_width, .constant = &E->width});
//
//    auto* four = (float*) malloc(sizeof(float));
//    *four = 4.0;
//
//    vector<CoPair> co_pairs_const;
//    co_pairs_const.push_back({1, &E->right});
//    system.add_constraint({.co_pairs = co_pairs_const, .constant = four});
//
////    print_system(system);
////    constraint_system_to_A_matrix(system).print();
//
//    float x = determinant(constraint_system_to_A_matrix(system));
//    printf("%f", x);
////    SquareMat inv = inverse(constraint_system_to_A_matrix(system));
////    inv.print();
//}

//void draw_rectangles() {
//    DrawRectangleRec(*E0, RED);
//    DrawRectangleRec(*E1, ORANGE);
//    DrawRectangleRec(*E2, YELLOW);
//    DrawRectangleRec(*E3, GREEN);
//    DrawRectangleRec(*E4, BLUE);
//}


struct CoPair {
    float coefficient;
    float *variable;
};

struct Constraint {
    vector<CoPair> co_pairs;
    float *constant;
};

struct ConstraintSystem {
    vector<float *> variables;
    vector<Constraint> constraints;

    void add_constraint(const Constraint &constraint) {
        for (CoPair co_pair: constraint.co_pairs) {
            if (find(variables.begin(), variables.end(), co_pair.variable) == variables.end()) {
                variables.push_back(co_pair.variable);
            }
        }

        constraints.push_back(constraint);

        if (constraints.size() > variables.size()) {
            printf("too many constraints");
            abort();
        }
    }
};

SquareMat constraint_system_to_A_matrix(ConstraintSystem constraint_system) {
    int variable_count = (int) constraint_system.variables.size();
    int matrix_element_count = variable_count * variable_count;
    auto *data = (float *) malloc(sizeof(float) * matrix_element_count);
    memset(data, 0, sizeof(float) * matrix_element_count);

    for (int r = 0; r < constraint_system.constraints.size(); r++) {
        Constraint constraint = constraint_system.constraints[r];
        for (CoPair co_pair: constraint.co_pairs) {
            vector<float *> v = constraint_system.variables;
            auto it = find(v.begin(), v.end(), co_pair.variable);
            auto c = it - v.begin();

            data[r * variable_count + c] = co_pair.coefficient;
        }
    }


    return {.data = data, .size = variable_count};
}

void print_system(ConstraintSystem system) {
    SquareMat mat = constraint_system_to_A_matrix(system);
    for (int r = 0; r < mat.size; r++) {
        for (int c = 0; c < mat.size; c++) {
            printf("%f ", mat.data[r * mat.size + c]);
        }
        printf("| %f\n", *system.constraints[r].constant);
    }

    printf("Variables: %zu\n", system.variables.size());
    printf("Constraints: %zu\n", system.constraints.size());
}