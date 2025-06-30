#include "ast/PrettyPrinter.h"
#include "ast/ASTVisitor.h"
#include "ast/AssignmentStatement.h"
#include "ast/BinaryExpression.h"
#include "ast/Identifier.h"
#include "ast/VariableDefinition.h"

namespace chocopy::ast {
  std::any PrettyPrinter::visit(const Program& ctx) {
    ASTVisitor::visit(ctx);

    return out.str();
  }

  std::any PrettyPrinter::visit(const Literal& ctx) {
    std::visit([this](auto&& value) {
      if constexpr (std::is_same_v<std::decay_t<decltype(value)>, bool>) {
        out << (value ? "True" : "False");
      } else if constexpr (std::is_same_v<std::decay_t<decltype(value)>, std::int32_t>) {
        out << value;
      } else if constexpr (std::is_same_v<std::decay_t<decltype(value)>, std::string>) {
        out << '"';
        for (const char& ch : value) {
          switch (ch) {
            case '\t': out << "\\t"; break;
            case '\n': out << "\\n"; break;
            case '\\': out << "\\\\"; break;
            case '"': out << "\\\""; break;
            default: out << ch;
          }
        }
        out << '"';
      }
    }, ctx.getValue());

    return {};
  }

  std::any PrettyPrinter::visit(const VariableDefinition& ctx) {
    out << std::get<std::string>(ctx.getName().getValue()) << ": " << ctx.getType()->getText() << " = ";
    ctx.getValue()->accept(*this);
    out << '\n';

    return {};
  }

  std::any PrettyPrinter::visit(const AssignmentStatement& ctx) {
    for (const auto& target : ctx.getTargets()) {
      out << std::get<std::string>(target.getName().getValue()) << " = ";
    }

    ctx.getExpr()->accept(*this);

    out << '\n';

    return {};
  }

  std::any PrettyPrinter::visit(const Identifier& ctx) {
    out << ctx.getName().str();

    return {};
  }

  std::any PrettyPrinter::visit(const BinaryExpression& ctx) {
    out << '(';
    ctx.getLHS()->accept(*this);

    switch (ctx.getOperator().getType()) {
      case TokenType::PLUS: out << " + "; break;
      case TokenType::MINUS: out << " - "; break;
      case TokenType::MULT: out << " * "; break;
      case TokenType::DIV: out << " // "; break;
      case TokenType::MOD: out << " % "; break;
    }

    ctx.getRHS()->accept(*this);
    out << ')';

    return {};
  }
}