#include "../ast.hpp"


namespace rust_lang::parser {

ASTWhile::~ASTWhile() {
  delete m_condition;
  delete m_scope;
}

ASTExpression* ASTWhile::condition() const {
  return m_condition;
}

ASTScope* ASTWhile::scope() const {
  return m_scope;
}

void ASTWhile::set_condition(ASTExpression* cond) {
  m_condition = cond;
}

void ASTWhile::set_scope(ASTScope* scope) {
  m_scope = scope;
}

void ASTWhile::accept(Visitor& visitor) {
  visitor.visit(*this);
}

}
