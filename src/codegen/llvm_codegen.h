#pragma once

#include "llvm_headers.h"
#include "variable.h"

VIEW_DECLARATIONS(llvm_store)
VIEW_DECLARATIONS(llvm_load)
VIEW_DECLARATIONS(llvm_add)
VIEW_DECLARATIONS(llvm_call)
VIEW_DECLARATIONS(llvm_ret)
VIEW_DECLARATIONS(llvm_br)
VIEW_DECLARATIONS(llvm_icmp)
VIEW_DECLARATIONS(llvm_zext)
VIEW_DECLARATIONS(llvm_phi)
VIEW_DECLARATIONS(llvm_alloca)
VIEW_DECLARATIONS(llvm_value)

llvm::Module *build_llvm(void* flow);

void supplement_llvm_module(llvm::Module *module);

void perform_pass(
    llvm::Module *module,
    std::function<void(llvm::Function&, llvm::FunctionAnalysisManager&)> pass
);

llvm::Value* codegen_expression(void* expression, llvm::IRBuilder<> *builder, llvm::LLVMContext *context, unordered_map<Variable*, llvm::AllocaInst*> *variable_map);
