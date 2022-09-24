#include "../ast.hpp"


namespace rust_lang::parser {

ASTFunction::~ASTFunction() {
  delete m_expression;
}


std::string ASTFunction::func_name() const {
  return m_func_name;
}
std::string ASTFunction::return_type() const {
  return m_return_type;
}

void ASTFunction::set_func_name(const std::string& func_name) {
  m_func_name = func_name;
}
void ASTFunction::set_return_type(const std::string& ret_type) {
  m_return_type = ret_type;
}

ASTValue* ASTFunction::argument(std::size_t i) const {
  return m_arguments[i];
}
std::vector<ASTValue*>& ASTFunction::arguments() {
  return m_arguments;
}
void ASTFunction::append_argument(ASTValue* arg) {
  m_arguments.push_back(arg);
}
std::size_t ASTFunction::size() const {
  return m_arguments.size();
}

ASTScope* ASTFunction::scope() {
  return m_expression;
}
void ASTFunction::set_scope(ASTScope* scope) {
  m_expression = scope;
}

void ASTFunction::accept(Visitor& visitor) {
  visitor.visit(*this);
}

}
