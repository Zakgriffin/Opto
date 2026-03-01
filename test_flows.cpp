#include "test_flows.h"

void* create_fib() {
    auto x = typed(DECLARE, new Declare);
    object_to_name.insert({x, "x"});
    auto y = typed(DECLARE, new Declare);
    object_to_name.insert({y, "y"});
    auto z = typed(DECLARE, new Declare);
    object_to_name.insert({z, "z"});

    return typed(LOOP, new Loop{.body =
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
                    .procedure = nullptr,
                    .param = z,
                }),
                .next = nullptr
                })})})}),
            .finally = nullptr,
            .condition = typed(GREATER_THAN, new GreaterThan{
                    .left = typed(INTEGER, new int(20)),
                    .right = z,
                })
            })
        })})})})})
    });
}