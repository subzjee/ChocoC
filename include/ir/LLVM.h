#pragma once

#include "ASTVisitor.h"
#include "parser/ParseContext.h"
#include "semantic/SymbolTable.h"

#include "llvm/IR/BasicBlock.h"
#include "llvm/IR/DerivedTypes.h"
#include "llvm/IR/Function.h"
#include "llvm/IR/IRBuilder.h"
#include "llvm/IR/LLVMContext.h"
#include "llvm/IR/Module.h"
#include "llvm/IR/Verifier.h"
#include "llvm/IR/GlobalVariable.h"
#include "llvm/Support/ErrorHandling.h"
#include "llvm/Support/Signals.h"

namespace chocopy {
class IRGen : ASTVisitor {
public:
  using ASTVisitor::visit;

  IRGen(const llvm::StringRef module_name, SymbolTable& symbol_table)
      : m_ctx{std::make_unique<llvm::LLVMContext>()}, m_builder{*m_ctx},
        m_module{
            std::make_unique<llvm::Module>(std::move(module_name), *m_ctx)},
        m_symbol_table(symbol_table) {};

  void prologue() {
    // Setup the main function.
    llvm::FunctionType* func_type =
        llvm::FunctionType::get(m_builder.getInt32Ty(), false);
    llvm::Function* entry_func = llvm::Function::Create(
        func_type, llvm::Function::ExternalLinkage, "entry", m_module.get());

    // Setup the initial basic block.
    llvm::BasicBlock* entry_bb =
        llvm::BasicBlock::Create(*m_ctx, "entry", entry_func);
    m_builder.SetInsertPoint(entry_bb);
  }

  void epilogue() {
    const auto ret_val =
        llvm::ConstantInt::get(llvm::Type::getInt32Ty(*m_ctx), 0);

    m_builder.CreateRet(ret_val);
    m_module->print(llvm::outs(), nullptr);
  }

  virtual std::any visit(const ProgramContext& ctx) override {
    prologue();

    ASTVisitor::visit(ctx);

    epilogue();

    return {};
  }

  virtual std::any visit(const LiteralContext& ctx) override {
    if (ctx.getType() == TokenType::INTLIT) {
      return static_cast<llvm::Constant*>(llvm::ConstantInt::get(Type::getIntegerType()->toLLVMType(*m_ctx), std::get<std::int32_t>(ctx.getValue())));
    } else if (ctx.getType() == TokenType::TRUE || ctx.getType() == TokenType::FALSE) {
      return static_cast<llvm::Constant*>(llvm::ConstantInt::get(Type::getBooleanType()->toLLVMType(*m_ctx), std::get<bool>(ctx.getValue())));
    }

    return {};
  }

  virtual std::any visit(const VarDefContext& ctx) override {
    const auto name = std::get<std::string>(ctx.getName().getValue());
    auto& variable = std::get<Variable>(m_symbol_table.getEntry(name)->get());

    llvm::Type* llvm_type = variable.type.toLLVMType(*m_ctx);
    llvm::Constant* init = std::any_cast<llvm::Constant*>(visit(*ctx.getValue()));

    if (scope == 0) {
      llvm::GlobalVariable* g = new llvm::GlobalVariable(
          *m_module, llvm_type, false, llvm::GlobalValue::ExternalLinkage, init,
          name);
      variable.allocation = g;
    }

    return {};
  }

  virtual std::any visit(const AssignmentStmtContext& ctx) override {
    const auto expr = std::any_cast<llvm::Constant*>(visit(*ctx.getExpr()));

    for (const auto& target : ctx.getTargets()) {
      const auto& entry = m_symbol_table.getEntry(
          std::get<std::string>(target.getName().getValue()));
      const auto& variable = std::get<Variable>(entry->get());

      m_builder.CreateStore(expr, variable.allocation);
    }

    return {};
  }

private:
  std::unique_ptr<llvm::LLVMContext> m_ctx;
  llvm::IRBuilder<> m_builder;
  std::unique_ptr<llvm::Module> m_module;

  SymbolTable& m_symbol_table;
  unsigned int scope = 0;
};
} // namespace chocopy