#include "../ast.hpp"


namespace rust_lang::parser {

ASTLoop::~ASTLoop() {
  delete m_scope;
}

ASTScope* ASTLoop::scope() const {
  return m_scope;
}

void ASTLoop::set_scope(ASTScope* scope) {
  m_scope = scope;
}

void ASTLoop::accept(Visitor& visitor) {
  visitor.visit(*this);
}

}
