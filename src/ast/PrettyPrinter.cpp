#include "ast/PrettyPrinter.h"
#include "ast/ASTVisitor.h"
#include "ast/AssignmentStatement.h"
#include "ast/BinaryExpression.h"
#include "ast/Identifier.h"
#include "ast/VariableDefinition.h"
#include <utility>

namespace chocopy::ast {
std::any PrettyPrinter::visit(const Program& ctx) {
  ASTVisitor::visit(ctx);

  return out.str();
}

std::any PrettyPrinter::visit(const Literal& ctx) {
  std::visit(
      [this](auto&& value) {
        if constexpr (std::is_same_v<std::decay_t<decltype(value)>, bool>) {
          out << (value ? "True" : "False");
        } else if constexpr (std::is_same_v<std::decay_t<decltype(value)>,
                                            std::int32_t>) {
          out << value;
        } else if constexpr (std::is_same_v<std::decay_t<decltype(value)>,
                                            std::string>) {
          out << '"';
          for (const char& ch : value) {
            switch (ch) {
            case '\t':
              out << "\\t";
              break;
            case '\n':
              out << "\\n";
              break;
            case '\\':
              out << "\\\\";
              break;
            case '"':
              out << "\\\"";
              break;
            default:
              out << ch;
            }
          }
          out << '"';
        }
      },
      ctx.getValue());

  return {};
}

std::any PrettyPrinter::visit(const VariableDefinition& ctx) {
  ctx.getName()->accept(*this);
  out << ": " << ctx.getType()->getText() << " = ";
  ctx.getValue()->accept(*this);
  out << '\n';

  return {};
}

std::any PrettyPrinter::visit(const AssignmentStatement& ctx) {
  for (const auto& target : ctx.getTargets()) {
    target->accept(*this);
    out << " = ";
  }

  ctx.getExpr()->accept(*this);

  out << '\n';

  return {};
}

std::any PrettyPrinter::visit(const Identifier& ctx) {
  out << ctx.getValue().str();

  return {};
}

std::any PrettyPrinter::visit(const BinaryExpression<ast::Expression>& ctx) {
  out << '(';
  ctx.getLHS()->accept(*this);

  switch (ctx.getOperator().getType()) {
  case TokenType::AND:
    out << " and ";
    break;
  case TokenType::OR:
    out << " or ";
    break;
  default:
    std::unreachable();
  }

  ctx.getRHS()->accept(*this);
  out << ')';

  return {};
}

std::any
PrettyPrinter::visit(const BinaryExpression<ast::ConstantExpression>& ctx) {
  out << '(';
  ctx.getLHS()->accept(*this);

  switch (ctx.getOperator().getType()) {
  case TokenType::PLUS:
    out << " + ";
    break;
  case TokenType::MINUS:
    out << " - ";
    break;
  case TokenType::MULT:
    out << " * ";
    break;
  case TokenType::DIV:
    out << " // ";
    break;
  case TokenType::MOD:
    out << " % ";
    break;
  case TokenType::EQUAL:
    out << " == ";
    break;
  case TokenType::NEQUAL:
    out << " != ";
    break;
  case TokenType::LESS:
    out << " < ";
    break;
  case TokenType::GREAT:
    out << " > ";
    break;
  case TokenType::LESSEQ:
    out << " <= ";
    break;
  case TokenType::GREATEQ:
    out << " >= ";
    break;
  default:
    std::unreachable();
  }

  ctx.getRHS()->accept(*this);
  out << ')';

  return {};
}

std::any PrettyPrinter::visit(const UnaryExpression<ast::Expression>& ctx) {
  out << '(';

  switch (ctx.getOperator().getType()) {
  case TokenType::NOT:
    out << "not ";
    break;
  default:
    std::unreachable();
  }

  ctx.getRHS()->accept(*this);
  out << ')';

  return {};
}

std::any
PrettyPrinter::visit(const UnaryExpression<ast::ConstantExpression>& ctx) {
  out << '(';

  switch (ctx.getOperator().getType()) {
  case TokenType::MINUS:
    out << "-";
    break;
  default:
    std::unreachable();
  }

  ctx.getRHS()->accept(*this);
  out << ')';

  return {};
}
} // namespace chocopy::ast