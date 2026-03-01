#include "llvm_compile.h"

// ZZZZ gross
#ifdef PI
#undef PI
#undef RED
#undef GREEN
#undef BLUE
#undef BLACK
#undef YELLOW
#undef MAGENTA
#undef WHITE
#endif


#include "llvm/IR/LLVMContext.h"
#include "llvm/IR/Module.h"
#include "llvm/IR/LegacyPassManager.h"

#include "llvm/Support/TargetSelect.h"
#include "llvm/Support/Host.h"
#include "llvm/Support/raw_ostream.h"
#include "llvm/Support/FileSystem.h"

#include "llvm/Target/TargetMachine.h"
#include "llvm/Target/TargetOptions.h"
#include "llvm/MC/TargetRegistry.h"

#include "llvm/IR/IRBuilder.h"
#include "llvm/IR/Verifier.h"

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

void build_llvm() {
    llvm::LLVMContext context;
    llvm::Module module("module", context);
    llvm::IRBuilder<> builder(context);

    include_print_int(context, module);

    auto fib = create_fib();
    // auto flow = removed_scope_flow(fib);
    auto flow = fib;


    llvm::Type* i32 = llvm::Type::getInt32Ty(context);

    llvm::FunctionType* funcType = llvm::FunctionType::get(llvm::Type::getVoidTy(context), false);
    llvm::Function* func = llvm::Function::Create(funcType, llvm::Function::ExternalLinkage, "main", module);

    llvm::BasicBlock* entry = llvm::BasicBlock::Create(context, "entry", func);
    builder.SetInsertPoint(entry);

    std::unordered_map<Declare*, llvm::AllocaInst*> variable_map;

    traverse_over_scopes(flow, [&](DoThen* each){
        if (object_to_type.at(each->effect) == DECLARE) {
            auto declare = (Declare*)each->effect;
            llvm::AllocaInst* alloca = builder.CreateAlloca(i32, nullptr, object_to_name.at(declare));
            variable_map.insert({declare, alloca});
        }
    },
    [&](void* each, ObjectType type){},
    [&](void* each, ObjectType type){}
    );

    map<void*, llvm::BasicBlock*> flow_to_basic_block;

    traverse_over_scopes(flow, [&](DoThen* each){
        auto type = object_to_type.at(each->effect);
         if (type == ASSIGN) {
             auto assign = (Assign*)each->effect;
             auto grantee = (Declare*)assign->grantee;
             llvm::Value* value_to_store = nullptr;

             auto grantor = assign->grantor;
             auto grantor_type = object_to_type.at(grantor);

             if (grantor_type == INTEGER) {
                 value_to_store = llvm::ConstantInt::get(i32, *(int*)grantor);
             } else if (grantor_type == DECLARE) {
                 value_to_store = builder.CreateLoad(i32, variable_map[(Declare*)grantor]);
             } else if (grantor_type == ADD) {
                 auto add = (Add*)grantor;
                 llvm::Value* left = builder.CreateLoad(i32, variable_map[(Declare*)add->augend]);
                 llvm::Value* right = builder.CreateLoad(i32, variable_map[(Declare*)add->addend]);
                 value_to_store = builder.CreateAdd(left, right);
             }

            builder.CreateStore(value_to_store, variable_map[grantee]);
         } else if (type == CALL) {
             auto call = (Call*) each->effect;
             auto param = (Declare*) call->param;
             // if (call->procedure == print_int) {
            llvm::Function *print_func = module.getFunction("print_int");
            llvm::Value *val = builder.CreateLoad(builder.getInt32Ty(), variable_map[param]);
            builder.CreateCall(print_func, { val });
             // }
         }
    }, [&](void* opened, ObjectType type){
        llvm::BasicBlock* bb = llvm::BasicBlock::Create(context, "block", func);
        flow_to_basic_block.insert({opened, bb});
        builder.CreateBr(bb);
        builder.SetInsertPoint(bb);
    },[&](void* closed, ObjectType type){
        if (type == LOOP) {
            llvm::BasicBlock* bb = llvm::BasicBlock::Create(context, "block", func);
            builder.CreateBr(flow_to_basic_block.at(closed));
            builder.SetInsertPoint(bb);
        } else if (type == REPEAT) {
            auto repeat = (Repeat*)closed;
            // ZZZZ bad casts
            auto gt = (GreaterThan*) repeat->condition;
            auto left = (int*)gt->left;
            auto right = (Declare*)gt->right;

            auto cond_val = variable_map[right];
            llvm::Value* cond_val_loaded = builder.CreateLoad(i32, cond_val);

            llvm::Value *cond = builder.CreateICmpSLT(cond_val_loaded,builder.getInt32(*left),"cond");

            llvm::BasicBlock* bb = llvm::BasicBlock::Create(context, "block", func);
            builder.CreateCondBr(cond, flow_to_basic_block.at(closed), bb);
            builder.SetInsertPoint(bb);
        } else {
            abort();
        }

        flow_to_basic_block.erase(closed);
    });

    builder.CreateRetVoid();

    llvm::verifyFunction(*func);



    llvm::outs() << "\n--- Generated LLVM IR ---\n";
    module.print(llvm::outs(), nullptr);
    llvm::outs() << "--------------------------\n\n";

    emit_module(module);
}