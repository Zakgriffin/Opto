#include "type.h"

bool is_num_exp(ObjectType type) {
    auto exps = unordered_set {
        ADD,
        SUB,
        AND,
        INTEGER,
        GREATER_THAN,
        LESS_THAN,
        GREATER_THAN_OR_EQUAL,
    };
    return exps.contains(type);
}

bool is_control_flow(ObjectType type) {
    auto control_flows = unordered_set {
        LOOP,
        REPEAT,
        WHILE,
        DO_THEN,
        NONE
    };
    return control_flows.contains(type);
}

ObjectType t(void* obj) {
    return object_to_type.at(obj);
}

void k(void* parent_obj, int sub_index, bool good) {
    if (!object_to_view.contains(parent_obj)) return;

    // auto parent_view = object_to_view.at(parent_obj);
    // auto view = parent_view->sub_object_views.at(sub_index);
    // if (good) {
    //     view->editable_text.underline_color = (ray::Color){ 0, 228, 48, 255 };
    // } else {
    //     view->editable_text.underline_color = (ray::Color){ 230, 41, 55, 255 };
    // }
}

bool is_effect(ObjectType type) {
    auto effects = unordered_set {
        ASSIGN,
        DECLARE,
        CALL,
    };
    return effects.contains(type);
}

ObjectType env_type(void* obj, map<void*, ObjectType> *env) {
    if (!env->contains(obj)) return NONE;
    return env->at(obj);
}

void type_check_binary_int(void* parent, void* left, void* right, map<void*, ObjectType>* env) {
    type_check(left, env);
    k(parent, 0, env_type(left, env) == INTEGER);
    type_check(right, env);
    k(parent, 1, env_type(right, env) == INTEGER);
    env->insert({parent, INTEGER});
}

void type_check(void *obj, map<void*, ObjectType> *env) {
    auto type = t(obj);

    if (type == NONE) {

    } else if (type == DECLARE) {

    } else if (type == LOOP) {
        auto loop = (Loop*) obj;
        type_check(loop->body, env);
        k(loop, 0, is_control_flow(t(loop->body)));
    } else if (type == DO_THEN) {
        auto do_then = (DoThen*) obj;
        type_check(do_then->effect, env);
        k(do_then, 0, is_effect(t(do_then->effect)));
        type_check(do_then->next, env);
        k(do_then, 1, is_control_flow(t(do_then->next)));
    } else if (type == REPEAT) {
        auto repeat = (Repeat*) obj;
        type_check(repeat->then, env);
        k(repeat, 0, is_control_flow(t(repeat->then)));
        type_check(repeat->condition, env);
        k(repeat, 1, env_type(repeat->condition, env) == INTEGER);
        type_check(repeat->finally, env);
        k(repeat, 2, is_control_flow(t(repeat->finally)));
    }  else if (type == WHILE) {
        auto while_ = (While*) obj;
        type_check(while_->condition, env);
        k(while_, 0, env_type(while_->condition, env) == INTEGER);
        type_check(while_->then, env);
        k(while_, 1, is_control_flow(t(while_->then)));
        type_check(while_->finally, env);
        k(while_, 2, is_control_flow(t(while_->finally)));
    } else if (type == ASSIGN) {
        auto assign = (Assign*) obj;
        type_check(assign->grantor, env);
        type_check(assign->grantee, env);

        bool grantor_good = true;
        auto grantee_env = env_type(assign->grantee, env);
        auto grantor_env = env_type(assign->grantor, env);

        if (grantee_env == NONE) {
            env->insert({assign->grantee, grantor_env});
        } else if (grantee_env != grantor_env) {
            grantor_good = false;
        }

        auto grantee_type = t(assign->grantee);
        k(assign, 0, grantee_type == DECLARE || grantee_type == INDEX);
        k(assign, 1, grantor_good);
    } else if (type == INTEGER) {
        auto integer = (int*) obj;
        env->insert({integer, INTEGER});
    } else if (type == ADD) {
        auto add = (Add*) obj;
        type_check_binary_int(add, add->addend, add->augend, env);
    } else if (type == SUB) {
        auto sub = (Sub*) obj;
        type_check_binary_int(sub, sub->minuend, sub->subtrahend, env);
    } else if (type == MUL) {
        auto mul = (Mul*) obj;
        type_check_binary_int(mul, mul->multiplicand, mul->multiplier, env);
    } else if (type == AND) {
        auto and_ = (And*) obj;
        type_check_binary_int(and_, and_->conjunct_1, and_->conjunct_2, env);
    } else if (type == GREATER_THAN) {
        auto greater_than = (GreaterThan*) obj;
        type_check_binary_int(greater_than, greater_than->left, greater_than->right, env);
    } else if (type == GREATER_THAN_OR_EQUAL) {
        auto greater_than_or_equal = (GreaterThanOrEqual*) obj;
        type_check_binary_int(greater_than_or_equal, greater_than_or_equal->left, greater_than_or_equal->right, env);
    } else if (type == LESS_THAN) {
        auto less_than = (LessThan*) obj;
        type_check_binary_int(less_than, less_than->left, less_than->right, env);
    } else if (type == CALL) {
        auto call = (Call*) obj;
        type_check(call->procedure, env);
        k(call, 0, t(call->procedure) == PROCEDURE);
        type_check(call->param, env);
        k(call, 1, true);
    } else if (type == INDEX) {
        auto index = (Index*) obj;
        type_check(index->array, env);
        k(index, 0, env_type(index->array, env) == VECTOR);
        type_check(index->at, env);
        k(index, 1, env_type(index->at, env) == INTEGER);
        env->insert({index, INTEGER});
    } else if (type == VECTOR) {
        auto vector_ = (vector<void*>*) obj;
        for (int i = 0; i < vector_->size(); i++) {
            type_check(vector_->at(i), env);
            k(vector_, i, t(vector_->at(i)) == INTEGER); // ZZZZ reasonable for now
        }
        env->insert({vector_, VECTOR});
    } else if (type == PROCEDURE) {
        auto procedure = (Procedure*) obj;
        type_check(procedure->param, env);
        env->insert({procedure->param, INTEGER});
        k(procedure, 0, env_type(procedure->param, env) == INTEGER); // ZZZZ wrong
        type_check(procedure->body, env);
        k(procedure, 1, is_control_flow(object_to_type.at(procedure->body)));
    } else {
        cout << "bad type check " << type << endl;
    }
}