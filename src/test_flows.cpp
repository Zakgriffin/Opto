#include "control_flow.h"
#include "data.h"
#include "expressions.h"
#include "object_view.h"
#include "procedures.h"
#include "test_flows.h"


void append(void** root_handle, void* effect) {
    *root_handle = typed(DO_THEN, new DoThen{.effect = effect, .next = nullptr});
}

void append_do_then(void*** current_handle, void* effect) {
    if (effect == nullptr) return;

    auto k = typed(DO_THEN, new DoThen{.effect = effect, .next = nullptr});
    **current_handle = k;
    *current_handle = &k->next;
}

void* sequence(std::vector<void*> statements) {
    void* head = nullptr;
    for (auto it = statements.rbegin(); it != statements.rend(); ++it) {
        void* current = *it;
        head = typed(DO_THEN, new DoThen{
            .effect = current,
            .next = head
        });
    }
    return head;
}

void* create_fib() {
    auto limit = typed(VARIABLE, new Variable);
    object_to_name.insert({limit, "limit"});

    auto x = typed(VARIABLE, new Variable);
    object_to_name.insert({x, "x"});
    auto y = typed(VARIABLE, new Variable);
    object_to_name.insert({y, "y"});
    auto z = typed(VARIABLE, new Variable);
    object_to_name.insert({z, "z"});

    auto print = typed(PROCEDURE, new Procedure{.param = nullptr, .body = nullptr});
    object_to_name.insert({print, "print"});

    return typed(PROCEDURE, new Procedure{
        .param = limit,
        .body = typed(LOOP, new Loop{.body =
            typed(DO_THEN, new DoThen{.effect = x,
                .next = typed(DO_THEN, new DoThen{.effect = y,
                .next = typed(DO_THEN, new DoThen{.effect = z,

                .next = typed(DO_THEN, new DoThen{.effect = typed(ASSIGN, new Assign{
                    .grantee = x,
                    .grantor = typed(INTEGER, new int(0))
                }),
                .next = typed(DO_THEN, new DoThen{.effect = typed(ASSIGN, new Assign{
                    .grantee = y,
                    .grantor = typed(INTEGER, new int(1))
                }),
                .next = typed(REPEAT, new Repeat{
                    .then = typed(DO_THEN, new DoThen{.effect = typed(ASSIGN, new Assign{
                        .grantee = z,
                        .grantor = typed(ADD, new Add{
                            .augend = x,
                            .addend = y,
                        })
                    }),
                    .next = typed(DO_THEN, new DoThen{.effect = typed(ASSIGN, new Assign{
                        .grantee = x,
                        .grantor = y,
                    }),
                    .next = typed(DO_THEN, new DoThen{.effect = typed(ASSIGN, new Assign{
                        .grantee = y,
                        .grantor = z,
                    }),
                    .next = typed(DO_THEN, new DoThen{.effect = typed(CALL, new Call{
                        .procedure = print,
                        .param = z,
                    }),
                    .next = nullptr
                    })})})}),
                .finally = nullptr,
                .condition = typed(GREATER_THAN, new GreaterThan{
                        .left = limit,
                        .right = z,
                    })
                })
            })})})})})
        })
    });
}

void* create_insertion_sort() {
    auto i = typed(VARIABLE, new Variable);
    object_to_name.insert({i, "i"});
    auto j = typed(VARIABLE, new Variable);
    object_to_name.insert({j, "j"});
    auto key = typed(VARIABLE, new Variable);
    object_to_name.insert({key, "key"});
    auto arr = typed(VARIABLE, new Variable);
    object_to_name.insert({arr, "arr"});

    auto print = typed(PROCEDURE, new Procedure{.param = nullptr, .body = nullptr});
    object_to_name.insert({print, "print"});

    auto arr_size = typed(INTEGER, new int(4));
    object_to_name.insert({arr_size, "arr_size"});

    auto inner_while = typed(WHILE, new While{
        .condition = typed(AND, new And{
            .conjunct_1 = typed(GREATER_THAN_OR_EQUAL, new GreaterThanOrEqual{.left = j, .right = typed(INTEGER, new int(0))}),
            .conjunct_2 = typed(GREATER_THAN, new GreaterThan{
                .left = typed(INDEX, new Index{.array = arr, .at = j}),
                .right = key
            })
        }),
        .then = sequence({
            typed(ASSIGN, new Assign{
                .grantee = typed(INDEX, new Index{.array = arr, .at = typed(ADD, new Add{.augend = j, .addend = typed(INTEGER, new int(1))})}),
                .grantor = typed(INDEX, new Index{.array = arr, .at = j})
            }),
            typed(ASSIGN, new Assign{
                .grantee = j,
                .grantor = typed(SUB, new Sub{.minuend = j, .subtrahend = typed(INTEGER, new int(1))})
            })
        }),
        .finally = sequence({
            typed(ASSIGN, new Assign{
                .grantee = typed(INDEX, new Index{.array = arr, .at = typed(ADD, new Add{.augend = j, .addend = typed(INTEGER, new int(1))})}),
                .grantor = key
            }),
            typed(ASSIGN, new Assign{
                .grantee = i,
                .grantor = typed(ADD, new Add{.augend = i, .addend = typed(INTEGER, new int(1))})
            })
        })
    });

    // --- Outer Loop Body ---
    auto outer_loop_body = typed(DO_THEN, new DoThen{
        .effect = typed(ASSIGN, new Assign{
            .grantee = key,
            .grantor = typed(INDEX, new Index{.array = arr, .at = i})
        }),
        .next = typed(DO_THEN, new DoThen{
            .effect = typed(ASSIGN, new Assign{
                .grantee = j,
                .grantor = typed(SUB, new Sub{.minuend = i, .subtrahend = typed(INTEGER, new int(1))})
            }),
            .next = inner_while // Control flow in .next
        })
    });

    auto outer_repeat = typed(REPEAT, new Repeat{
        .then = outer_loop_body,
        .finally = typed(DO_THEN, new DoThen{
            .effect = nullptr,
            .next = nullptr
        }),
        .condition = typed(LESS_THAN, new LessThan{.left = i, .right = arr_size})
    });

    return typed(PROCEDURE, new Procedure{
        .param = arr,
        .body = typed(DO_THEN, new DoThen{
            .effect = i,
            .next = typed(DO_THEN, new DoThen{
                .effect = j,
                .next = typed(DO_THEN, new DoThen{
                    .effect = key,
                    .next = typed(DO_THEN, new DoThen{
                        .effect = typed(ASSIGN, new Assign{
                            .grantee = i,
                            .grantor = typed(INTEGER, new int(1))
                        }),
                        .next = outer_repeat
                    })
                })
            })
        })
    });
}

void* create_matrix_multiplication() {
    // Variable Declarations
    auto a = typed(VARIABLE, new Variable);
    object_to_name.insert({a, "a"});
    auto b = typed(VARIABLE, new Variable);
    object_to_name.insert({b, "b"});
    auto c = typed(VARIABLE, new Variable);
    object_to_name.insert({c, "c"});

    auto i = typed(VARIABLE, new Variable);
    object_to_name.insert({i, "i"});
    auto j = typed(VARIABLE, new Variable);
    object_to_name.insert({j, "j"});
    auto k = typed(VARIABLE, new Variable);
    object_to_name.insert({k, "k"});

    auto sum = typed(VARIABLE, new Variable);
    object_to_name.insert({sum, "sum"});

    auto size = typed(VARIABLE, new Variable);
    object_to_name.insert({size, "size"});

    // --- Inner Dot Product Loop (k) ---
    auto k_while = typed(WHILE, new While{
        .condition = typed(LESS_THAN, new LessThan{.left = k, .right = size}),
        .then = typed(DO_THEN, new DoThen{
            .effect = typed(ASSIGN, new Assign{
                .grantee = sum,
                .grantor = typed(ADD, new Add{
                    .augend = sum,
                    .addend = typed(MUL, new Mul{
                        // a[i * size + k]
                        .multiplicand = typed(INDEX, new Index{
                            .array = a,
                            .at = typed(ADD, new Add{
                                .augend = typed(MUL, new Mul{.multiplicand = i, .multiplier = size}),
                                .addend = k
                            })
                        }),
                        // b[k * size + j]
                        .multiplier = typed(INDEX, new Index{
                            .array = b,
                            .at = typed(ADD, new Add{
                                .augend = typed(MUL, new Mul{.multiplicand = k, .multiplier = size}),
                                .addend = j
                            })
                        })
                    })
                })
            }),
            .next = typed(DO_THEN, new DoThen{
                .effect = typed(ASSIGN, new Assign{
                    .grantee = k,
                    .grantor = typed(ADD, new Add{.augend = k, .addend = typed(INTEGER, new int(1))})
                }),
                .next = nullptr
            })
        })
    });

    // --- Middle Loop (j) ---
    auto j_while = typed(WHILE, new While{
        .condition = typed(LESS_THAN, new LessThan{.left = j, .right = size}),
        .then = typed(DO_THEN, new DoThen{
            .effect = typed(ASSIGN, new Assign{.grantee = sum, .grantor = typed(INTEGER, new int(0))}),
            .next = typed(DO_THEN, new DoThen{
                .effect = typed(ASSIGN, new Assign{.grantee = k, .grantor = typed(INTEGER, new int(0))}),
                .next = k_while
            })
        })
    });

    j_while->finally = sequence({
        typed(ASSIGN, new Assign{
            // c[i * size + j]
            .grantee = typed(INDEX, new Index{
                .array = c,
                .at = typed(ADD, new Add{
                    .augend = typed(MUL, new Mul{.multiplicand = i, .multiplier = size}),
                    .addend = j
                })
            }),
            .grantor = sum
        }),
        typed(ASSIGN, new Assign{
            .grantee = j,
            .grantor = typed(ADD, new Add{.augend = j, .addend = typed(INTEGER, new int(1))})
        })
    });

    // Adding the increment/assignment for j into the .finally of j_while or as a terminal .next
    j_while->finally = sequence({
        typed(ASSIGN, new Assign{
            // c[i * size + j]
            .grantee = typed(INDEX, new Index{
                .array = c,
                .at = typed(ADD, new Add{
                    .augend = typed(MUL, new Mul{.multiplicand = i, .multiplier = size}),
                    .addend = j
                })
            }),
            .grantor = sum
        }),
        typed(ASSIGN, new Assign{
            .grantee = j,
            .grantor = typed(ADD, new Add{.augend = j, .addend = typed(INTEGER, new int(1))})
        })
    });

    // --- Outer Loop (i) ---
    auto i_while = typed(WHILE, new While{
        .condition = typed(LESS_THAN, new LessThan{.left = i, .right = size}),
        .then = typed(DO_THEN, new DoThen{
            .effect = typed(ASSIGN, new Assign{.grantee = j, .grantor = typed(INTEGER, new int(0))}),
            .next = j_while
        })
    });

    i_while->finally = typed(DO_THEN, new DoThen{
        .effect = typed(ASSIGN, new Assign{
            .grantee = i,
            .grantor = typed(ADD, new Add{.augend = i, .addend = typed(INTEGER, new int(1))})
        }),
        .next = nullptr
    });

    // --- Main Procedure Structure ---
    return typed(PROCEDURE, new Procedure{
        .param = a,
        .body = typed(DO_THEN, new DoThen{ .effect = i,
        .next = typed(DO_THEN, new DoThen{ .effect = j,
        .next = typed(DO_THEN, new DoThen{ .effect = size,
        .next = typed(DO_THEN, new DoThen{ .effect = k,
        .next = typed(DO_THEN, new DoThen{ .effect = sum,
        .next = typed(DO_THEN, new DoThen{ .effect = b,
        .next = typed(DO_THEN, new DoThen{ .effect = c,
        .next = typed(DO_THEN, new DoThen{
            .effect = typed(ASSIGN, new Assign{.grantee = i, .grantor = typed(INTEGER, new int(0))}),
            .next = i_while
        })})})})})})})})
    });
}

void* create_big_expression() {
    void* current;
    void** current_handle = &current;
    auto result = &current;

    auto x = typed(VARIABLE, new Variable);
    auto a= typed(VARIABLE, new Variable);

    append_do_then(&current_handle, typed(INTEGER, new int(0)));
    append_do_then(&current_handle, typed(ASSIGN, new Assign{
        .grantee = x,
        .grantor = typed(ADD, new Add{
            .augend = typed(ADD, new Add{
                .augend = typed(INTEGER, new int(1)),
                .addend = a
            }),
            .addend = typed(INTEGER, new int(5))
        })
    }));
    append_do_then(&current_handle, typed(INTEGER, new int(0)));

    return *result;
}