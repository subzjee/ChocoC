#include "ir/LLVM.h"
#include "ast/BinaryExpression.h"
#include "ast/Identifier.h"
#include <any>

namespace chocopy {
void IRGen::prologue() {
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

void IRGen::epilogue() {
  const auto ret_val =
      llvm::ConstantInt::get(llvm::Type::getInt32Ty(*m_ctx), 0);

  m_builder.CreateRet(ret_val);
  m_module->print(llvm::outs(), nullptr);
}

std::any IRGen::visit(const ast::Program& ctx) {
  prologue();

  ASTVisitor::visit(ctx);

  epilogue();

  return {};
}

std::any IRGen::visit(const ast::Literal& ctx) {
  if (ctx.getType() == "int") {
    return static_cast<llvm::Value*>(llvm::ConstantInt::get(Type::getIntegerType()->toLLVMType(*m_ctx),
                                std::get<std::int32_t>(ctx.getValue())));
  } else if (ctx.getType() == "bool") {
    return static_cast<llvm::Value*>(llvm::ConstantInt::get(Type::getBooleanType()->toLLVMType(*m_ctx),
                                std::get<bool>(ctx.getValue())));
  }

  return {};
}

std::any IRGen::visit(const ast::VariableDefinition& ctx) {
  const auto name = std::get<std::string>(ctx.getName().getValue());
  auto& variable = std::get<Variable>(m_symbol_table.getEntry(name)->get());

  llvm::Type* llvm_type = variable.type.toLLVMType(*m_ctx);

  // A variable definition's value can only be a literal, which we know is a Constant*
  // so we static_cast it back to a Constant*.
  llvm::Constant* init =
      static_cast<llvm::Constant*>(std::any_cast<llvm::Value*>(visit(*ctx.getValue())));

  if (scope == 0) {
    llvm::GlobalVariable* g = new llvm::GlobalVariable(
        *m_module, llvm_type, false, llvm::GlobalValue::ExternalLinkage, init,
        name);
    variable.allocation = g;
  }

  return {};
}

std::any IRGen::visit(const ast::AssignmentStatement& ctx) {
  const auto expr = std::any_cast<llvm::Value*>(ctx.getExpr()->accept(*this));

  for (const auto& target : ctx.getTargets()) {
    const auto& entry = m_symbol_table.getEntry(
        std::get<std::string>(target.getName().getValue()));
    const auto& variable = std::get<Variable>(entry->get());

    m_builder.CreateStore(expr, variable.allocation);
  }

  return {};
}

std::any IRGen::visit(const ast::BinaryExpression& ctx) {
  llvm::Value* lhs = std::any_cast<llvm::Value*>(ctx.getLHS()->accept(*this));
  llvm::Value* rhs = std::any_cast<llvm::Value*>(ctx.getRHS()->accept(*this));

  switch (ctx.getOperator().getType()) {
    case TokenType::PLUS: return m_builder.CreateAdd(lhs, rhs);
    case TokenType::MINUS: return m_builder.CreateSub(lhs, rhs);
    case TokenType::MULT: return m_builder.CreateMul(lhs, rhs);
    case TokenType::DIV: return m_builder.CreateSDiv(lhs, rhs);
    case TokenType::MOD: return m_builder.CreateSRem(lhs, rhs);
  }

  return {};
}
}