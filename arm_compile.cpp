#include "arm_compile.h"

VIEW_DEFINITIONS_SIMPLE(conditional_jump, CONDITIONAL_JUMP, ConditionalJump, "conditional-jump", &ConditionalJump::condition, &ConditionalJump::jump)
VIEW_DEFINITIONS_SIMPLE(jump, JUMP, Jump, "jump", &Jump::jump)

// register

VIEW_DEFINITIONS_SINGLE(arm_register, ARM_REGISTER, ArmRegister, "REG")

// instructions

VIEW_DEFINITIONS_SIMPLE(arm_move_status, ARM_MOVE_STATUS, ArmMoveStatus, "MOVS", &ArmMoveStatus::reg_moved, &ArmMoveStatus::immediate)
VIEW_DEFINITIONS_SIMPLE(arm_add, ARM_ADD, ArmAdd, "ADD", &ArmAdd::reg_augend, &ArmAdd::reg_addend, &ArmAdd::immediate)
VIEW_DEFINITIONS_SIMPLE(arm_subtract, ARM_SUBTRACT, ArmSubtract, "SUB", &ArmSubtract::reg_minuend, &ArmSubtract::reg_subtrahend, &ArmSubtract::immediate)
VIEW_DEFINITIONS_SIMPLE(arm_load_register, ARM_LOAD_REGISTER, ArmLoadRegister, "LDR", &ArmLoadRegister::reg_stored, &ArmLoadRegister::reg_address, &ArmLoadRegister::offset)
VIEW_DEFINITIONS_SIMPLE(arm_store_register, ARM_STORE_REGISTER, ArmStoreRegister, "STR", &ArmStoreRegister::reg_stored, &ArmStoreRegister::reg_address, &ArmStoreRegister::offset)
VIEW_DEFINITIONS_SIMPLE(arm_compare, ARM_COMPARE, ArmCompare, "CMP", &ArmCompare::reg_compared, &ArmCompare::immediate)
VIEW_DEFINITIONS_SIMPLE(arm_branch, ARM_BRANCH, ArmBranch, "B", &ArmBranch::label);
VIEW_DEFINITIONS_SIMPLE(arm_branch_less_than_or_equal, ARM_BRANCH_LESS_THAN_OR_EQUAL, ArmBranchLessThanOrEqual, "BLE", &ArmBranchLessThanOrEqual::label)

// rest

void append(void** root_handle, void* effect) {
    *root_handle = typed(DO_THEN, new DoThen{.effect = effect, .next = nullptr});
}

void append_do_then(void*** current_handle, void* effect) {
    if (effect == nullptr) return;

    auto k = typed(DO_THEN, new DoThen{.effect = effect, .next = nullptr});
    **current_handle = k;
    *current_handle = &k->next;
}

void* stackify_variables(void* flow) {
    void* current;
    void** current_handle = &current;
    auto result = &current;

    vector<Declare*> variables;
    traverse_over_flat(flow, [&](DoThen* each){
        auto type = object_to_type.at(each->effect);
        if (type == DECLARE) {
            auto declare = (Declare*) each->effect;
            variables.push_back(declare);
        }
    });

    unordered_map<Declare*, int> stack_offsets;
    for (int i = 0; i < variables.size(); i++) {
        auto variable = variables.at(i);
        stack_offsets.insert({variable, (variables.size()-i)*4});
    }

    // ZZZZ dumb silly
    auto sp = typed(ARM_REGISTER, new ArmRegister);
    object_to_name.insert({sp, "sp"});
    auto r7 = typed(ARM_REGISTER, new ArmRegister);
    object_to_name.insert({r7, "r7"});

    stack<ArmRegister*> registers;
    for (int i = 6; i >= 2; i--) {
        auto rn = typed(ARM_REGISTER, new ArmRegister);
        object_to_name.insert({rn, "r"  + to_string(i)});
        registers.push(rn);
    }
    auto grab_register = [&] {
        if (registers.empty()) {
            cout << "NO MORE REGISTERS!" << endl;
        }
        auto reg = registers.top();
        registers.pop();
        return reg;
    };
    auto release_register = [&](ArmRegister* reg) {
        registers.push(reg);
    };


    auto build_load_register = [&](void* reg_stored, Declare* declare){
        auto offset = typed(INTEGER, new int(stack_offsets.at(declare)));
        return typed(ARM_LOAD_REGISTER, new ArmLoadRegister{.reg_stored = reg_stored, .reg_address = r7, .offset = offset});
    };

    // ZZZ dont trust this
    auto pushed_registers = 1;
    auto fp_reserved = 1;
    auto stack_size = (variables.size() + pushed_registers + fp_reserved) * 4;
    stack_size = (stack_size + 7) & ~7; // round up multiple of 8
    stack_size -= 4;

    auto arm_subtract_stack = typed(ARM_SUBTRACT, new ArmSubtract{.reg_minuend = sp, .reg_subtrahend = sp, .immediate = typed(INTEGER, new int(stack_size))});
    append_do_then(&current_handle, arm_subtract_stack);

    auto arm_add_stack = typed(ARM_ADD, new ArmAdd{.reg_augend = r7, .reg_addend = sp, .immediate = typed(INTEGER, new int(0))});
    append_do_then(&current_handle, arm_add_stack);

    traverse_over_flat(flow, [&](DoThen* each){
        auto type = object_to_type.at(each->effect);
        if (type == ASSIGN) {
            auto assign = (Assign*) each->effect;
            auto grantee = (Declare*) assign->grantee;

            auto reg = grab_register();

            auto grantor = assign->grantor;
            auto grantor_type = object_to_type.at(grantor);
            // ZZZZ reuse expression stuff
            if (grantor_type == INTEGER){
                auto integer = (int*) grantor;
                auto arm_move_status = typed(ARM_MOVE_STATUS, new ArmMoveStatus{.reg_moved = reg, .immediate =  integer});
                append_do_then(&current_handle, arm_move_status);
            } else if (grantor_type == DECLARE){
                auto declare = (Declare*) grantor;
                append_do_then(&current_handle, build_load_register(reg, declare));
            } else if (grantor_type == ADD){
                auto add = (Add*) grantor;

                auto reg_2 = grab_register();

                append_do_then(&current_handle, build_load_register(reg, (Declare*) add->augend));
                append_do_then(&current_handle, build_load_register(reg_2, (Declare*) add->addend));

                auto offset = typed(INTEGER, new int(0));
                auto arm_add = typed(ARM_ADD, new ArmAdd{.reg_augend = reg, .reg_addend = reg_2, .immediate = offset});
                append_do_then(&current_handle, arm_add);

                release_register(reg_2);
            } else {
                cout << "BAD ASSIGN GUY" << endl;
            }

            auto immediate = typed(INTEGER, new int(stack_offsets.at(grantee)));
            auto arm_store_register = typed(ARM_STORE_REGISTER, new ArmStoreRegister{.reg_stored = reg, .reg_address = r7, .offset = immediate});
            append_do_then(&current_handle, arm_store_register);

            release_register(reg);
        } else if (type == JUMP) {
            auto arm_branch = typed(ARM_BRANCH, new ArmBranch{.label = nullptr});
            append_do_then(&current_handle, arm_branch);
        } else if (type == CONDITIONAL_JUMP) {
            auto conditional_jump = (ConditionalJump*) each->effect;
            auto condition = conditional_jump->condition;
            auto condition_type = object_to_type.at(condition);
            if (condition_type == GREATER_THAN) {
                auto greater_than = (GreaterThan*) condition;

                auto reg = grab_register();

                // ZZZZ bad assumed expression stuff
                auto constant_compared = (int*) greater_than->left; // ZZZZ this-1, whatever
                auto var = (Declare*) greater_than->right;

                auto offset =  typed(INTEGER, new int(stack_offsets.at(var)));
                auto arm_load_register = typed(ARM_LOAD_REGISTER, new ArmLoadRegister{.reg_stored = reg, .reg_address = r7, .offset = offset});
                append_do_then(&current_handle, arm_load_register);

                auto arm_compare = typed(ARM_COMPARE, new ArmCompare{.reg_compared = reg, .immediate = constant_compared});
                append_do_then(&current_handle, arm_compare);

                auto arm_branch_less_than_or_equal = typed(ARM_BRANCH_LESS_THAN_OR_EQUAL, new ArmBranchLessThanOrEqual{.label = nullptr});
                append_do_then(&current_handle, arm_branch_less_than_or_equal);

                release_register(reg);
            } else {
                cout << "BAD CONDITION" << endl;
            }

        }
    });
    return *result;
}

void expand_expression(void*** current_handle, Declare* variable, void* expression) {
    auto type = object_to_type.at(expression);

    if (type == ADD) {
        auto add = (Add*) expression;

        if (object_to_type.at(add->augend) == DECLARE && object_to_type.at(add->addend) == DECLARE) {
            goto yeah_whatever_jump; // ZZZZ bad on purpose so i fix
        }

        auto left = typed(DECLARE, new Declare);
        expand_expression(current_handle, left, add->augend);

        auto right = typed(DECLARE, new Declare);
        expand_expression(current_handle, right, add->addend);

        append_do_then(current_handle, typed(ASSIGN, new Assign{
            .grantee = variable,
            .grantor = typed(ADD, new Add{.augend = left, .addend = right})
        }));
    }
yeah_whatever_jump:
    append_do_then(current_handle, typed(ASSIGN, new Assign{
        .grantee = variable,
        .grantor = expression
    }));
}

void* expand_expressions(void* flow) {
    void* current;
    void** current_handle = &current;
    auto result = &current;

    traverse_over_flat(flow, [&](DoThen* each){
        auto type = object_to_type.at(each->effect);
        if (type == ASSIGN) {
            auto assign = (Assign*) each->effect;
            expand_expression(&current_handle, (Declare*)assign->grantee, assign->grantor);
        } else {
            append_do_then(&current_handle, each->effect);
        }
    });
    return *result;
}