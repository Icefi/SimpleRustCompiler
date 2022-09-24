#include "../ast.hpp"


namespace rust_lang::parser {
  
ASTCall::~ASTCall() {
  for (auto& arg : arguments()) {
    delete arg;
  }
}

std::string ASTCall::name() const {
  return m_func_name;
}

void ASTCall::set_name(const std::string& name) {
  m_func_name = name;
}

std::string ASTCall::type() const { return m_type; }

void ASTCall::set_type(const std::string& type) { m_type = type; }

ASTExpression* ASTCall::argument(std::size_t i) const {
  return m_arguments[i];
}

std::vector<ASTExpression*>& ASTCall::arguments() {
  return m_arguments;
}

void ASTCall::append_argument(ASTExpression* arg) {
  m_arguments.push_back(arg);
}

std::size_t ASTCall::size() const {
  return m_arguments.size();
}

void ASTCall::accept(Visitor& visitor) {
  visitor.visit(*this);
}

}