#include "ir/LLVM.h"

namespace chocopy {
llvm::Value* IRGen::createLoadOrConstant(llvm::Value* allocation) {
  if (auto global = llvm::dyn_cast<llvm::GlobalVariable>(allocation)) {
    return cast<llvm::Value>(
        m_builder.CreateLoad(global->getValueType(), allocation));
  }

  return allocation;
}

void IRGen::prologue() {
  // Setup the main function.
  llvm::FunctionType* func_type =
      llvm::FunctionType::get(m_builder.getInt32Ty(), false);
  llvm::Function* entry_func = llvm::Function::Create(
      func_type, llvm::Function::ExternalLinkage, "main", m_module.get());

  // Setup the initial basic block.
  llvm::BasicBlock* entry_bb =
      llvm::BasicBlock::Create(*m_ctx, "main", entry_func);
  m_builder.SetInsertPoint(entry_bb);
}

void IRGen::epilogue() {
  const auto ret_val =
      llvm::ConstantInt::get(llvm::Type::getInt32Ty(*m_ctx), 0);

  m_builder.CreateRet(ret_val);
}

std::any IRGen::visit(const ast::Program& ctx) {
  prologue();

  ASTVisitor::visit(ctx);

  epilogue();

  return {};
}

std::any IRGen::visit(const ast::Literal& ctx) {
  if (ctx.getType() == "int") {
    return cast<llvm::Value>(
        llvm::ConstantInt::get(llvm::Type::getInt32Ty(*m_ctx),
                               std::get<std::int32_t>(ctx.getValue())));
  } else if (ctx.getType() == "bool") {
    return cast<llvm::Value>(llvm::ConstantInt::get(
        llvm::Type::getInt1Ty(*m_ctx), std::get<bool>(ctx.getValue())));
  } else if (ctx.getType() == "str") {
    const std::string text = std::get<std::string>(ctx.getValue());

    if (!m_string_allocations.contains(text)) {
      m_string_allocations[text] =
          m_builder.CreateGlobalString(std::get<std::string>(ctx.getValue()),
                                       ".intern.str", 0, m_module.get());
    }

    return cast<llvm::Value>(m_string_allocations[text]);
  }

  return {};
}

std::any IRGen::visit(const ast::Identifier& ctx) {
  const auto entry = m_symbol_table.getEntry(ctx.getValue());
  assert(entry && "Identifier not stored in symbol table");
  assert(std::holds_alternative<Variable>(entry->get()) &&
         "Entry is not a variable");

  auto variable = std::get<Variable>(entry->get());

  return variable.allocation;
}

std::any IRGen::visit(const ast::VariableDefinition& ctx) {
  const auto name = ctx.getName()->getValue();
  auto& variable = std::get<Variable>(m_symbol_table.getEntry(name)->get());

  // A variable definition's value can only be a literal, which we know is a
  // Constant* so we static_cast it back to a Constant*.
  llvm::Constant* init =
      cast<llvm::Constant>(std::any_cast<llvm::Value*>(visit(*ctx.getValue())));

  if (scope == 0) {
    llvm::GlobalVariable* g = new llvm::GlobalVariable(
        *m_module, init->getType(), false, llvm::GlobalValue::ExternalLinkage,
        init, name);
    variable.allocation = g;
  }

  return {};
}

std::any IRGen::visit(const ast::AssignmentStatement& ctx) {
  const auto expr = createLoadOrConstant(
      std::any_cast<llvm::Value*>(ctx.getExpr()->accept(*this)));

  for (const auto& target : ctx.getTargets()) {
    llvm::Value* allocation =
        std::any_cast<llvm::Value*>(target->accept(*this));
    assert(allocation && "Variable allocation is nullptr");

    m_builder.CreateStore(expr, allocation);
  }

  return {};
}

std::any IRGen::visit(const ast::BinaryExpression<ast::Expression>& ctx) {
  llvm::Value* lhs = createLoadOrConstant(
      std::any_cast<llvm::Value*>(ctx.getLHS()->accept(*this)));
  llvm::Value* rhs = createLoadOrConstant(
      std::any_cast<llvm::Value*>(ctx.getRHS()->accept(*this)));

  switch (ctx.getOperator().getType()) {
  case TokenType::AND:
    return m_builder.CreateAnd(lhs, rhs);
  case TokenType::OR:
    return m_builder.CreateOr(lhs, rhs);
  default:
    std::unreachable();
  }

  return {};
}

std::any
IRGen::visit(const ast::BinaryExpression<ast::ConstantExpression>& ctx) {
  llvm::Value* lhs = createLoadOrConstant(
      std::any_cast<llvm::Value*>(ctx.getLHS()->accept(*this)));
  llvm::Value* rhs = createLoadOrConstant(
      std::any_cast<llvm::Value*>(ctx.getRHS()->accept(*this)));

  switch (ctx.getOperator().getType()) {
  case TokenType::PLUS:
    return m_builder.CreateAdd(lhs, rhs);
  case TokenType::MINUS:
    return m_builder.CreateSub(lhs, rhs);
  case TokenType::MULT:
    return m_builder.CreateMul(lhs, rhs);
  case TokenType::DIV:
    return m_builder.CreateSDiv(lhs, rhs);
  case TokenType::MOD:
    return m_builder.CreateSRem(lhs, rhs);
  case TokenType::EQUAL:
    return m_builder.CreateICmpEQ(lhs, rhs);
  case TokenType::NEQUAL:
    return m_builder.CreateICmpNE(lhs, rhs);
  case TokenType::LESS:
    return m_builder.CreateICmpSLT(lhs, rhs);
  case TokenType::GREAT:
    return m_builder.CreateICmpSGT(lhs, rhs);
  case TokenType::LESSEQ:
    return m_builder.CreateICmpSLE(lhs, rhs);
  case TokenType::GREATEQ:
    return m_builder.CreateICmpSGE(lhs, rhs);
  default:
    std::unreachable();
  }

  return {};
}

std::any IRGen::visit(const ast::UnaryExpression<ast::Expression>& ctx) {
  llvm::Value* rhs = createLoadOrConstant(
      std::any_cast<llvm::Value*>(ctx.getRHS()->accept(*this)));

  switch (ctx.getOperator().getType()) {
  case TokenType::NOT:
    return m_builder.CreateNot(rhs);
  default:
    std::unreachable();
  }

  return {};
}

std::any
IRGen::visit(const ast::UnaryExpression<ast::ConstantExpression>& ctx) {
  llvm::Value* rhs = createLoadOrConstant(
      std::any_cast<llvm::Value*>(ctx.getRHS()->accept(*this)));

  switch (ctx.getOperator().getType()) {
  case TokenType::MINUS:
    return m_builder.CreateNeg(rhs);
  default:
    std::unreachable();
  }

  return {};
}

std::any IRGen::visit(const ast::GroupingExpression& ctx) {
  return ctx.getExpression()->accept(*this);
}
} // namespace chocopy