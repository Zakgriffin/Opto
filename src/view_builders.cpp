#include "view_builders.h"
#include "none.h"
#include "expressions.h"
#include "control_flow.h"
#include "run.h"
#include "data.h"
#include "procedures.h"
#include "compile.h"
#include "codegen.h"
#include "arm_codegen.h"
#include "llvm.h"
#include "test_flows.h"

void init_object_view_builders() {
    object_view_builders.push_back(none_object_view_builder);
    object_view_builders.push_back(do_then_object_view_builder);
    object_view_builders.push_back(add_object_view_builder);
    object_view_builders.push_back(sub_object_view_builder);
    object_view_builders.push_back(mul_object_view_builder);
    object_view_builders.push_back(and_object_view_builder);
    object_view_builders.push_back(assign_object_view_builder);
    object_view_builders.push_back(run_object_view_builder);
    object_view_builders.push_back(integer_object_view_builder);
    object_view_builders.push_back(string_object_view_builder);
    object_view_builders.push_back(variable_object_view_builder);
    object_view_builders.push_back(if_object_view_builder);
    object_view_builders.push_back(loop_object_view_builder);
    object_view_builders.push_back(while_object_view_builder);
    object_view_builders.push_back(repeat_object_view_builder);
    object_view_builders.push_back(greater_than_object_view_builder);
    object_view_builders.push_back(greater_than_or_equal_object_view_builder);
    object_view_builders.push_back(less_than_object_view_builder);
    object_view_builders.push_back(procedure_object_view_builder);
    object_view_builders.push_back(call_object_view_builder);
    object_view_builders.push_back(vector_object_view_builder);
    object_view_builders.push_back(index_object_view_builder);
    object_view_builders.push_back(compile_object_view_builder);

    object_view_builders.push_back(conditional_jump_object_view_builder);
    object_view_builders.push_back(jump_object_view_builder);

    object_view_builders.push_back(arm_register_object_view_builder);

    object_view_builders.push_back(arm_move_status_object_view_builder);
    object_view_builders.push_back(arm_add_object_view_builder);
    object_view_builders.push_back(arm_subtract_object_view_builder);
    object_view_builders.push_back(arm_load_register_object_view_builder);
    object_view_builders.push_back(arm_store_register_object_view_builder);
    object_view_builders.push_back(arm_compare_object_view_builder);
    object_view_builders.push_back(arm_branch_object_view_builder);
    object_view_builders.push_back(arm_branch_less_than_or_equal_object_view_builder);

    object_view_builders.push_back(llvm_module_object_view_builder);
    object_view_builders.push_back(llvm_function_object_view_builder);
    object_view_builders.push_back(llvm_basic_block_object_view_builder);

    object_view_builders.push_back(llvm_store_object_view_builder);
    object_view_builders.push_back(llvm_load_object_view_builder);
    object_view_builders.push_back(llvm_add_object_view_builder);
    object_view_builders.push_back(llvm_call_object_view_builder);
    object_view_builders.push_back(llvm_ret_object_view_builder);
    object_view_builders.push_back(llvm_br_object_view_builder);
    object_view_builders.push_back(llvm_icmp_object_view_builder);
    object_view_builders.push_back(llvm_zext_object_view_builder);
    object_view_builders.push_back(llvm_phi_object_view_builder);
    object_view_builders.push_back(llvm_alloca_object_view_builder);

    object_to_name.insert({create_fib(), "fib"});
    object_to_name.insert({create_insertion_sort(), "sort"});
    object_to_name.insert({create_matrix_multiplication(), "mat-mul"});
    // object_to_name.insert({removed_scope_flow(create_fib()), "flat"});
    // object_to_name.insert({stackify_variables(removed_scope_flow(create_fib())), "stack"});
    // object_to_name.insert({build_llvm(create_fib()), "llvm"});

    // registerPass("sroa", [](llvm::Function &F, llvm::FunctionAnalysisManager &FAM) {
    //     llvm::SROAPass(llvm::SROAOptions::ModifyCFG).run(F, FAM);
    // });
    // registerPass("inst-combine", [](llvm::Function &F, llvm::FunctionAnalysisManager &FAM) {
    //     llvm::InstCombinePass().run(F, FAM);
    // });
    // registerPass("reassociate", [](llvm::Function &F, llvm::FunctionAnalysisManager &FAM) {
    //     llvm::ReassociatePass().run(F, FAM);
    // });
    // registerPass("simplify-cfg", [](llvm::Function &F, llvm::FunctionAnalysisManager &FAM) {
    //     llvm::SimplifyCFGPass().run(F, FAM);
    // });
    // registerPass("adce", [](llvm::Function &F, llvm::FunctionAnalysisManager &FAM) {
    //     llvm::ADCEPass().run(F, FAM);
    // });
}
