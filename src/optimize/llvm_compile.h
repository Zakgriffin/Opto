#pragma once

#ifndef OPTO_LLVM_COMPILE_H
#define OPTO_LLVM_COMPILE_H

#include "test_flows.h"
#include "compile.h"
#include "declare.h"

#include "llvm/IR/LLVMContext.h"
#include "llvm/IR/Module.h"
#include "llvm/IR/LegacyPassManager.h"

#include "llvm/Support/TargetSelect.h"
#include "llvm/TargetParser/Host.h"
#include "llvm/Support/raw_ostream.h"
#include "llvm/Support/FileSystem.h"

#include "llvm/Target/TargetMachine.h"
#include "llvm/Target/TargetOptions.h"
#include "llvm/MC/TargetRegistry.h"

#include "llvm/IR/IRBuilder.h"
#include "llvm/IR/Verifier.h"

#include "llvm/IR/PassManager.h"
#include "llvm/Passes/PassBuilder.h"
#include "llvm/Transforms/Scalar/SROA.h"
#include "llvm/Transforms/InstCombine/InstCombine.h"
#include "llvm/Transforms/Scalar/SimplifyCFG.h"
#include "llvm/Transforms/Scalar/Reassociate.h"
#include "llvm/Transforms/Scalar/ADCE.h"


VIEW_DECLARATIONS(llvm_module)

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

llvm::Value* codegen_expression(void* expression, llvm::IRBuilder<> *builder, llvm::LLVMContext *context, map<Declare*, llvm::AllocaInst*> *variable_map);

#endif //OPTO_LLVM_COMPILE_H