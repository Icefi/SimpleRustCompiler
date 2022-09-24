#include "../ast.hpp"


namespace rust_lang::parser {

ASTFor::~ASTFor() {
  delete m_variable;
  delete m_first;
  delete m_last;
}

ASTValue* ASTFor::variable() const {
  return m_variable;
}
ASTExpression* ASTFor::first() const {
  return m_first;
}
ASTExpression* ASTFor::last() const {
  return m_last;
}
ASTScope* ASTFor::scope() const {
  return m_scope;
}

void ASTFor::set_variable(ASTValue* var) {
  m_variable = var;
}
void ASTFor::set_first(ASTExpression* first) {
  m_first = first;
}
void ASTFor::set_last(ASTExpression* last) {
  m_last = last;
}
void ASTFor::set_scope(ASTScope* scope) {
  m_scope = scope;
}

void ASTFor::accept(Visitor& visitor) {
  visitor.visit(*this);
}

}
