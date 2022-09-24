#include "../ast.hpp"


namespace rust_lang::parser {

ASTNode* ASTAssign::lvalue() const {
  return m_lvalue;
}
ASTNode* ASTAssign::rvalue() const {
  return m_rvalue;
}

void ASTAssign::set_lvalue(ASTNode* node) {
  m_lvalue = node;
}
void ASTAssign::set_rvalue(ASTNode* node) {
  m_rvalue = node;
}

void ASTAssign::accept(Visitor& visitor) {
  visitor.visit(*this);
}

}
