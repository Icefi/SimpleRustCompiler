#include "../ast.hpp"


namespace rust_lang::parser {

ASTReturn::~ASTReturn() {
  delete m_value;
}

ASTNode* ASTReturn::value() const {
  return m_value;
}
void ASTReturn::set_value(ASTNode* value) {
  m_value = value;
}

void ASTReturn::accept(Visitor& visitor) {
  visitor.visit(*this);
}

}
