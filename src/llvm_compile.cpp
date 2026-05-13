#include "llvm_compile.h"

int temp_name = 0;
string best_name(llvm::Value* value) {
    if (value->hasName()) {
        return value->getName().str();
    }

    return to_string(temp_name++);
}

void supplement_llvm_instruction(llvm::Instruction *inst) {
    switch (inst->getOpcode()) {
        case llvm::Instruction::Store: {
            auto *store = llvm::cast<llvm::StoreInst>(inst);
            object_to_type.insert({store, LLVM_STORE});
            object_to_name.insert({store, best_name(store)});

            auto value_operand = store->getValueOperand();
            if (!object_to_type.contains(value_operand)) object_to_type.insert({value_operand, LLVM_VALUE});
            object_to_name.insert({value_operand, best_name(value_operand)});

            auto pointer_operand = store->getPointerOperand();
            if (!object_to_type.contains(pointer_operand)) object_to_type.insert({pointer_operand, LLVM_VALUE});
            object_to_name.insert({pointer_operand, best_name(pointer_operand)});
            break;
        }
        case llvm::Instruction::Load: {
            auto *load = llvm::cast<llvm::LoadInst>(inst);
            object_to_type.insert({load, LLVM_LOAD});
            object_to_name.insert({load, best_name(load)});

            auto pointer_operand = load->getPointerOperand();
            if (!object_to_type.contains(pointer_operand)) object_to_type.insert({pointer_operand, LLVM_VALUE});
            object_to_name.insert({pointer_operand, best_name(pointer_operand)});
            break;
        }
        case llvm::Instruction::Add: {
            auto *add = llvm::cast<llvm::BinaryOperator>(inst);
            object_to_type.insert({add, LLVM_ADD});
            object_to_name.insert({add, best_name(add)});

            llvm::Value* lhs = add->getOperand(0);
            if (!object_to_type.contains(lhs))  object_to_type.insert({lhs, LLVM_VALUE});
            object_to_name.insert({lhs, best_name(lhs)});
            llvm::Value* rhs = add->getOperand(1);
            if (!object_to_type.contains(rhs)) object_to_type.insert({rhs, LLVM_VALUE});

            break;
        }
        case llvm::Instruction::Call: {
            auto *call = llvm::cast<llvm::CallInst>(inst);
            object_to_type.insert({call, LLVM_CALL});
            object_to_name.insert({call, best_name(call)});

            auto called_function = call->getCalledFunction();
            if (!object_to_type.contains(called_function)) object_to_type.insert({called_function, LLVM_FUNCTION});
            object_to_name.insert({called_function, best_name(called_function)});
            break;
        }
        case llvm::Instruction::Ret: {
            auto *ret = llvm::cast<llvm::ReturnInst>(inst);
            object_to_type.insert({ret, LLVM_RET});
            object_to_name.insert({ret, best_name(ret)});
            break;
        }
        case llvm::Instruction::Br: {
            auto *br = llvm::cast<llvm::BranchInst>(inst);
            object_to_type.insert({br, LLVM_BR});
            object_to_name.insert({br, best_name(br)});
            break;
        }
        case llvm::Instruction::ICmp: {
            auto *icmp = llvm::cast<llvm::ICmpInst>(inst);
            object_to_type.insert({icmp, LLVM_ICMP});
            object_to_name.insert({icmp, best_name(icmp)});
            break;
        }
        case llvm::Instruction::PHI: {
            auto *phi = llvm::cast<llvm::PHINode>(inst);
            object_to_type.insert({phi, LLVM_PHI});
            object_to_name.insert({phi, best_name(phi)});
            break;
        }
        case llvm::Instruction::Alloca: {
            auto *alloca = llvm::cast<llvm::AllocaInst>(inst);
            object_to_type.insert({alloca, LLVM_ALLOCA});
            object_to_name.insert({alloca, best_name(alloca)});
            break;
        }
        case llvm::Instruction::ZExt: {
            auto *zext = llvm::cast<llvm::ZExtInst>(inst);
            object_to_type.insert({zext, LLVM_ZEXT});
            object_to_name.insert({zext, best_name(zext)});

            // Optional: If you need to track the operand being extended
            auto source_operand = zext->getOperand(0);
            if (!object_to_type.contains(source_operand)) {
                object_to_type.insert({source_operand, LLVM_VALUE});
            }
            object_to_name.insert({source_operand, best_name(source_operand)});
            break;
        }
        default: {
            object_to_type.insert({inst, NONE});
            cout << "unknown llvm instruction " << inst->getOpcodeName() << endl;
            break;
        }
    }
}

void supplement_llvm_module(llvm::Module *module) {
    object_to_type.insert({module, LLVM_MODULE});

    for (auto &function : *module) {
        auto f = &function;
        object_to_type.insert({f, LLVM_FUNCTION});

        for (auto &basic_block : *f) {
            auto b = &basic_block;
            object_to_type.insert({b, LLVM_BASIC_BLOCK});
            object_to_name.insert({b, best_name(b)});
            for (auto &instruction : *b) {
                supplement_llvm_instruction(&instruction);
            }
        }
    }
}

void* null_ref = nullptr;

auto pre_name = [](void* i) -> void** {
    auto name = object_to_name.contains(i) ? object_to_name.at(i) : "?";
    auto s = typed(STRING, new string(name));
    return (void**) &s;
};

vector<function<void**(llvm::StoreInst*)>> llvm_store_getters {
    pre_name,
    [](llvm::StoreInst* i) -> void** {
        llvm::Value* value = i->getValueOperand();
        auto p = new void*;
        *p = static_cast<void*>(value);
        return p;
    },
    [](llvm::StoreInst* i) -> void** {
        llvm::Value* value = i->getPointerOperand();
        auto p = new void*;
        *p = static_cast<void*>(value);
        return p;
    }
};

vector<function<void**(llvm::LoadInst*)>> llvm_load_getters {
    pre_name,
    [](llvm::LoadInst* i) -> void** {
        llvm::Value* value = i->getPointerOperand();
        auto p = new void*;
        *p = static_cast<void*>(value);
        return p;
    }
};

vector<function<void**(llvm::BinaryOperator*)>> llvm_add_getters {
    pre_name,
    [](llvm::BinaryOperator* i) -> void** {
        llvm::Value* value = i->getOperand(0);
        auto p = new void*;
        *p = static_cast<void*>(value);
        return p;
    },
    [](llvm::BinaryOperator* i) -> void** {
        llvm::Value* value = i->getOperand(1);
        auto p = new void*;
        *p = static_cast<void*>(value);
        return p;
    }
};

vector<function<void**(llvm::CallInst*)>> llvm_call_getters {
    pre_name,
    [](llvm::CallInst* i) -> void** {
        llvm::Value* value = i->getCalledFunction();
        auto p = new void*;
        *p = static_cast<void*>(value);
        return p;
    }
};

vector<function<void**(llvm::ReturnInst*)>> llvm_ret_getters {
    pre_name,
    [](llvm::ReturnInst* i) -> void** {
        llvm::Value* value = i->getReturnValue();
        auto p = new void*;
        *p = static_cast<void*>(value);
        return p;
    }
};

vector<function<void**(llvm::BranchInst*)>> llvm_br_getters {
    pre_name,
    [](llvm::BranchInst* i) -> void** {
        llvm::Value* value = nullptr;
        if (i->isConditional()) {
            value = i->getCondition();
        } else {
            value = i->getSuccessor(0);
        }

        auto p = new void*;
        *p = static_cast<void*>(value);
        return p;
    },
    [](llvm::BranchInst* i) -> void** {
        if (i->isConditional()) {
            llvm::Value* value = i->getSuccessor(0);
            auto p = new void*;
            *p = static_cast<void*>(value);
            return p;
        }
        return &null_ref;
    },
    [](llvm::BranchInst* i) -> void** {
        if (i->isConditional()) {
            llvm::Value* value = i->getSuccessor(1);
            auto p = new void*;
            *p = static_cast<void*>(value);
            return p;
        }
        return &null_ref;
    }
};

vector<function<void**(llvm::ICmpInst*)>> llvm_icmp_getters {
    pre_name,
    [](llvm::ICmpInst* i) -> void** {
        llvm::Value* value = i->getOperand(0);
        auto p = new void*;
        *p = static_cast<void*>(value);
        return p;
    },
    [](llvm::ICmpInst* i) -> void** {
        llvm::Value* value = i->getOperand(1);
        auto p = new void*;
        *p = static_cast<void*>(value);
        return p;
    }
};

vector<function<void**(llvm::PHINode*)>> llvm_phi_getters {
    pre_name,
    [](llvm::PHINode* i) -> void** {
        if (i->getNumIncomingValues() > 0) {
            llvm::Value* value = i->getIncomingValue(0);
            auto p = new void*;
            *p = static_cast<void*>(value);
            return p;
        }
        return &null_ref;
    },
    [](llvm::PHINode* i) -> void** {
        if (i->getNumIncomingValues() > 1) {
            llvm::Value* value = i->getIncomingValue(1);
            auto p = new void*;
            *p = static_cast<void*>(value);
            return p;
        }
        return &null_ref;
    }
};

vector<function<void**(llvm::ZExtInst*)>> llvm_zext_getters {
    pre_name,
    [](llvm::ZExtInst* i) -> void** {
        llvm::Value* value = i->getOperand(0);
        auto p = new void*;
        *p = static_cast<void*>(value);
        return p;
    }
};

vector<function<void**(llvm::AllocaInst*)>> llvm_alloca_getters {
    pre_name,
    [](llvm::AllocaInst* i) -> void** {
        llvm::Value* value = i->getArraySize();
        auto p = new void*;
        *p = static_cast<void*>(value);
        return p;
    }
};

VIEW_DEFINITIONS_GETTERS(llvm_store, LLVM_STORE, llvm::StoreInst, "store", llvm_store_getters)
VIEW_DEFINITIONS_GETTERS(llvm_load, LLVM_LOAD, llvm::LoadInst, "load", llvm_load_getters)
VIEW_DEFINITIONS_GETTERS(llvm_add, LLVM_ADD, llvm::BinaryOperator, "add", llvm_add_getters)
VIEW_DEFINITIONS_GETTERS(llvm_call, LLVM_CALL, llvm::CallInst, "call", llvm_call_getters)
VIEW_DEFINITIONS_GETTERS(llvm_ret, LLVM_RET, llvm::ReturnInst, "ret", llvm_ret_getters)
VIEW_DEFINITIONS_GETTERS(llvm_br, LLVM_BR, llvm::BranchInst, "br", llvm_br_getters)
VIEW_DEFINITIONS_GETTERS(llvm_icmp, LLVM_ICMP, llvm::ICmpInst, "icmp", llvm_icmp_getters)
VIEW_DEFINITIONS_GETTERS(llvm_zext, LLVM_ZEXT, llvm::ZExtInst, "zext", llvm_zext_getters)
VIEW_DEFINITIONS_GETTERS(llvm_phi, LLVM_PHI, llvm::PHINode, "phi", llvm_phi_getters)
VIEW_DEFINITIONS_GETTERS(llvm_alloca, LLVM_ALLOCA, llvm::AllocaInst, "alloca", llvm_alloca_getters)
VIEW_DEFINITIONS_GETTERS(llvm_value, LLVM_VALUE, int, "value", {})

std::string getModuleIR(llvm::Module &M) {
    std::string buffer;
    llvm::raw_string_ostream rso(buffer);
    M.print(rso, nullptr);
    return buffer;
}

void perform_pass(
    llvm::Module *module,
    std::function<void(llvm::Function&, llvm::FunctionAnalysisManager&)> pass
) {
    llvm::LoopAnalysisManager LAM;
    llvm::FunctionAnalysisManager FAM;
    llvm::CGSCCAnalysisManager CGAM;
    llvm::ModuleAnalysisManager MAM;

    llvm::PassBuilder PB;
    PB.registerModuleAnalyses(MAM);
    PB.registerCGSCCAnalyses(CGAM);
    PB.registerFunctionAnalyses(FAM);
    PB.registerLoopAnalyses(LAM);
    PB.crossRegisterProxies(LAM, FAM, CGAM, MAM);

    // for (llvm::Function &F : *module) {
    //     if (F.isDeclaration()) continue;
    //     pass(F, FAM);
    //
    // }

    llvm::Function *lastFunc = nullptr;
    for (auto it = module->rbegin(); it != module->rend(); ++it) {
        if (!it->isDeclaration()) {
            lastFunc = &*it;
            break;
        }
    }

    auto getFuncSize = [](llvm::Function *F) -> int {
        if (!F) return 0;
        // Approximation of size via instruction count
        // For actual bitcode bytes, we'd need to emit the function to a sub-module
        return (size_t)F->getInstructionCount();
    };

    if (lastFunc) {
        llvm::outs() << "Last function [" << lastFunc->getName() << "] size before: "
                     << getFuncSize(lastFunc) << " instructions\n";
    }

    for (llvm::Function &F : *module) {
        if (F.isDeclaration()) continue;
        pass(F, FAM);
    }

    if (lastFunc) {
        llvm::outs() << "Last function [" << lastFunc->getName() << "] size after: "
                     << getFuncSize(lastFunc) << " instructions\n";
    }
}

void emit_module(llvm::Module &module) {
    llvm::InitializeNativeTarget();
    llvm::InitializeNativeTargetAsmPrinter();
    llvm::InitializeNativeTargetAsmParser();

    std::string targetTripleStr = llvm::sys::getDefaultTargetTriple();
    llvm::Triple targetTriple(targetTripleStr);
    module.setTargetTriple(targetTriple);

    std::string error;
    const llvm::Target* target =
        llvm::TargetRegistry::lookupTarget(targetTriple, error);

    if (!target) {
        llvm::errs() << error;
        return;
    }

    llvm::TargetOptions opt;
    auto cpu = "generic";
    auto features = "";

    auto targetMachine =
        target->createTargetMachine(targetTriple, cpu, features, opt, llvm::Reloc::PIC_);

    module.setDataLayout(targetMachine->createDataLayout());

    std::error_code EC;
    llvm::raw_fd_ostream dest("output.o", EC, llvm::sys::fs::OF_None);

    if (EC) {
        llvm::errs() << "Could not open file: " << EC.message();
        return;
    }

    llvm::legacy::PassManager pass;

    if (targetMachine->addPassesToEmitFile(pass, dest, nullptr, llvm::CodeGenFileType::ObjectFile)) {
        llvm::errs() << "TargetMachine can't emit object file";
        return;
    }

    pass.run(module);
    dest.flush();

    llvm::outs() << "Wrote output.o\n";

    // ---- Link to executable ----
    int result = std::system("clang output.o -o program");
    if (result != 0) {
        llvm::errs() << "Linking failed\n";
        return;
    }
    llvm::outs() << "Created executable: program\n";
}

void include_print_int(llvm::LLVMContext &context, llvm::Module &module) {
    llvm::IRBuilder<> builder(context);

    llvm::FunctionType *printfType = llvm::FunctionType::get(builder.getInt32Ty(),builder.getPtrTy(),true);

    llvm::Function *printfFunc = llvm::Function::Create(printfType,llvm::Function::ExternalLinkage,"printf", module);
    llvm::FunctionType *printType = llvm::FunctionType::get(builder.getVoidTy(),{ builder.getInt32Ty() },false );

    llvm::Function *print_func = llvm::Function::Create(printType,llvm::Function::ExternalLinkage,"print_int",module);

    llvm::BasicBlock *entry = llvm::BasicBlock::Create(context, "entry", print_func);
    builder.SetInsertPoint(entry);

    llvm::Function::arg_iterator args = print_func->arg_begin();
    llvm::Value *x = args++;
    x->setName("x");

    llvm::Value *formatStr = builder.CreateGlobalStringPtr("%d\n", "fmt");

    builder.CreateCall(printfFunc,{ formatStr, x });

    builder.CreateRetVoid();
}

llvm::Value* codegen_expression(void* expression, llvm::IRBuilder<> *builder, llvm::LLVMContext *context, map<Declare*, llvm::AllocaInst*> *variable_map) {
    auto type = object_to_type.at(expression);
    auto i32 = llvm::Type::getInt32Ty(*context);
    auto ptr_type = llvm::PointerType::get(*context, 0);

    if (type == INTEGER) {
        return llvm::ConstantInt::get(i32, *(int*)expression);
    }
    if (type == DECLARE) {
        return builder->CreateLoad(i32, variable_map->at((Declare*)expression));
    }
    if (type == ADD) {
        auto add = (Add*)expression;
        llvm::Value* left = codegen_expression(add->augend, builder, context, variable_map);
        llvm::Value* right = codegen_expression(add->addend, builder, context, variable_map);
        return builder->CreateAdd(left, right);
    }
    if (type == SUB) {
        auto sub = (Sub*)expression;
        llvm::Value* left = codegen_expression(sub->minuend, builder, context, variable_map);
        llvm::Value* right = codegen_expression(sub->subtrahend, builder, context, variable_map);
        return builder->CreateSub(left, right);
    }
    if (type == MUL) {
        auto mul = (Mul*)expression;
        llvm::Value* left = codegen_expression(mul->multiplicand, builder, context, variable_map);
        llvm::Value* right = codegen_expression(mul->multiplier, builder, context, variable_map);
        return builder->CreateMul(left, right);
    }
    if (type == AND) {
        auto and_ = (And*)expression;
        llvm::Value* left = codegen_expression(and_->conjunct_1, builder, context, variable_map);
        llvm::Value* right = codegen_expression(and_->conjunct_2, builder, context, variable_map);
        return builder->CreateAnd(left, right);
    }
    if (type == GREATER_THAN) {
        auto greater_than = (GreaterThan*)expression;
        llvm::Value* left = codegen_expression(greater_than->left, builder, context, variable_map);
        llvm::Value* right = codegen_expression(greater_than->right, builder, context, variable_map);
        llvm::Value* cmp = builder->CreateICmpSGT(left, right);
        return builder->CreateZExt(cmp, i32);
    }
    if (type == GREATER_THAN_OR_EQUAL) {
        auto greater_than_or_equal = (GreaterThanOrEqual*)expression;
        llvm::Value* left = codegen_expression(greater_than_or_equal->left, builder, context, variable_map);
        llvm::Value* right = codegen_expression(greater_than_or_equal->right, builder, context, variable_map);
        llvm::Value* cmp = builder->CreateICmpSGE(left, right);
        return builder->CreateZExt(cmp, i32);
    }
    if (type == LESS_THAN) {
        auto less_than = (LessThan*)expression;
        llvm::Value* left = codegen_expression(less_than->left, builder, context, variable_map);
        llvm::Value* right = codegen_expression(less_than->right, builder, context, variable_map);
        llvm::Value* cmp = builder->CreateICmpSLT(left, right);
        return builder->CreateZExt(cmp, i32);
    }
    if (type == INDEX) {
        auto index = (Index*)expression;
        llvm::Value* array_ptr = builder->CreateLoad(ptr_type, variable_map->at((Declare*)index->array));
        llvm::Value* index_val = codegen_expression(index->at, builder, context, variable_map);
        llvm::Value* element_ptr = builder->CreateGEP(i32, array_ptr, index_val);
        return builder->CreateLoad(i32, element_ptr);
    }
    if (type == VECTOR) {
        auto v = (vector<void*>*) expression;
        auto size = v->size();

        llvm::Value* array_size = llvm::ConstantInt::get(i32, size);
        llvm::Value* allocated_array = builder->CreateAlloca(i32, array_size);

        for (int i = 0; i < size; i++) {
            llvm::Value* element_value = codegen_expression(v->at(i), builder, context, variable_map);

            llvm::Value* index_val = llvm::ConstantInt::get(i32, i);
            llvm::Value* element_ptr = builder->CreateGEP(i32, allocated_array, index_val);

            builder->CreateStore(element_value, element_ptr);
        }
        return allocated_array;
    }

    cout << "bad value in expression " << type << endl;
    return nullptr;
}

llvm::Module *build_llvm(void* proc_) {
    auto proc = (Procedure*) proc_;

    auto context = new llvm::LLVMContext;
    auto module = new llvm::Module("module", *context);
    llvm::IRBuilder<> builder(*context);

    include_print_int(*context, *module);

    llvm::Type* i32 = llvm::Type::getInt32Ty(*context);
    auto ptr_type = llvm::PointerType::get(*context, 0);

    llvm::FunctionType* procType = llvm::FunctionType::get(i32, {i32}, false);
    string procName = object_to_name.contains(proc) ? object_to_name.at(proc) : "main";
    llvm::Function* func = llvm::Function::Create(procType, llvm::Function::ExternalLinkage, procName, module);

    llvm::BasicBlock* entry = llvm::BasicBlock::Create(*context, "entry", func);
    builder.SetInsertPoint(entry);

    std::map<Declare*, llvm::AllocaInst*> variable_map;
    map<void*, ObjectType> env;

    void* flow = proc->body;
    type_check(flow, &env);

    if (proc->param) {
        auto* param_decl = (Declare*)proc->param;
        string pName = object_to_name.contains(param_decl) ? object_to_name.at(param_decl) : "param";
        llvm::AllocaInst* pAlloca = builder.CreateAlloca(i32, nullptr, pName);
        builder.CreateStore(func->arg_begin(), pAlloca);
        variable_map[param_decl] = pAlloca;
    }

    traverse_over_scopes(flow, [&](DoThen* each){
        if (object_to_type.at(each->effect) == DECLARE) {
            auto declare = (Declare*)each->effect;
            if (declare == (Declare*)proc->param) return;

            auto name = object_to_name.contains(declare) ? object_to_name.at(declare) : "var";
            if (env_type(declare, &env) == VECTOR) {
                variable_map[declare] = builder.CreateAlloca(ptr_type, nullptr, name + "_ptr");
            } else {
                variable_map[declare] = builder.CreateAlloca(i32, nullptr, name);
            }
        }
    }, [](void*, ObjectType){}, [](void*, ObjectType){});

    map<void*, llvm::BasicBlock*> flow_to_basic_block;

    traverse_over_scopes(flow, [&](DoThen* each){
        auto type = object_to_type.at(each->effect);
        if (type == ASSIGN) {
            auto assign = (Assign*)each->effect;
            llvm::Value* val = codegen_expression(assign->grantor, &builder, context, &variable_map);
            if (object_to_type.at(assign->grantee) == INDEX) {
                auto idx = (Index*)assign->grantee;
                llvm::Value* arr = builder.CreateLoad(ptr_type, variable_map.at((Declare*)idx->array));
                llvm::Value* i = codegen_expression(idx->at, &builder, context, &variable_map);
                builder.CreateStore(val, builder.CreateGEP(i32, arr, i));
            } else {
                builder.CreateStore(val, variable_map[(Declare*)assign->grantee]);
            }
        } else if (type == CALL) {
            auto call = (Call*)each->effect;
            llvm::Value* arg = builder.CreateLoad(i32, variable_map[(Declare*)call->param]);

            // ZZZZ garbag
            llvm::Function *print_func = module->getFunction("print_int");
            builder.CreateCall(print_func, { arg });
        }
    }, [&](void* opened, ObjectType){
        llvm::BasicBlock* bb = llvm::BasicBlock::Create(*context, "block", func);
        flow_to_basic_block[opened] = bb;
        builder.CreateBr(bb);
        builder.SetInsertPoint(bb);
    }, [&](void* closed, ObjectType type){
        if (type == LOOP) {
            builder.CreateBr(flow_to_basic_block.at(closed));
            builder.SetInsertPoint(llvm::BasicBlock::Create(*context, "after_loop", func));
        } else if (type == REPEAT) {
            auto repeat = (Repeat*)closed;
            llvm::Value* cond = builder.CreateICmpNE(codegen_expression(repeat->condition, &builder, context, &variable_map), llvm::ConstantInt::get(i32, 0));
            llvm::BasicBlock* next = llvm::BasicBlock::Create(*context, "after_repeat", func);
            builder.CreateCondBr(cond, flow_to_basic_block.at(closed), next);
            builder.SetInsertPoint(next);
        } else if (type == WHILE) {
            auto while_ = (While*)closed;
            llvm::BasicBlock* header = llvm::BasicBlock::Create(*context, "while_header", func);
            llvm::BasicBlock* after = llvm::BasicBlock::Create(*context, "after_while", func);
            builder.CreateBr(header);
            builder.SetInsertPoint(header);
            llvm::Value* cond = builder.CreateICmpNE(codegen_expression(while_->condition, &builder, context, &variable_map), llvm::ConstantInt::get(i32, 0));
            builder.CreateCondBr(cond, flow_to_basic_block.at(closed), after);
            builder.SetInsertPoint(after);
        }
    });

    // Using func->back() to safely access the tail block
    llvm::BasicBlock &lastBB = func->back();
    if (!lastBB.getTerminator()) {
        builder.SetInsertPoint(&lastBB);
        builder.CreateRet(llvm::ConstantInt::get(i32, 0));
    }

    llvm::verifyFunction(*func);

    llvm::outs() << "\n--- Generated LLVM IR ---\n";
    module->print(llvm::outs(), nullptr);

    // emit_module(*module);
    supplement_llvm_module(module);

    return module;
}