#include "../ast.hpp"


namespace rust_lang::parser {

ASTIf::~ASTIf() {
  delete m_if_condition;
  delete m_if_scope;
  delete m_else_scope;

  for (auto& cond : m_elif_conditions) {
    delete cond;
  }

  for (auto& scope : m_elif_scopes) {
    delete scope;
  }
}

ASTExpression* ASTIf::if_cond() {
  return m_if_condition;
}
ASTScope* ASTIf::if_scope() {
  return m_if_scope;
}
ASTExpression* ASTIf::elif_cond(std::size_t i) {
  return m_elif_conditions[i];
}
std::vector<ASTExpression*>& ASTIf::elif_conds() {
  return m_elif_conditions;
}
ASTScope* ASTIf::elif_scope(std::size_t i) {
  return m_elif_scopes[i];
}
std::vector<ASTScope*>& ASTIf::elif_scopes() {
  return m_elif_scopes;
}
ASTScope* ASTIf::else_scope() {
  return m_else_scope;
}

void ASTIf::set_if_cond(ASTExpression* cond) {
  m_if_condition = cond;
}
void ASTIf::set_if_scope(ASTScope* scope) {
  m_if_scope = scope;
}
void ASTIf::append_cond(ASTExpression* expr) {
  m_elif_conditions.push_back(expr);
}
void ASTIf::append_scope(ASTScope* scope) {
  m_elif_scopes.push_back(scope);
}
void ASTIf::set_else_scope(ASTScope* scope) {
  m_else_scope = scope;
}

void ASTIf::accept(Visitor& visitor) {
  visitor.visit(*this);
}

}