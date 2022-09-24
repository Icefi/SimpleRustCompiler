#include "../ast.hpp"


namespace rust_lang::parser {

ASTProgram::~ASTProgram() {
  for (unsigned int i = 0; i < size(); i++) {
    delete m_children[i];
  }
}

void ASTProgram::append_child(ASTNode* node) {
  m_children.push_back(node);
}
ASTNode* ASTProgram::child(std::size_t i) const {
  return m_children[i];
}

std::vector<ASTNode*>& ASTProgram::children() {
  return m_children;
}

std::size_t ASTProgram::size() const {
  return m_children.size();
}

void ASTProgram::accept(Visitor& visitor) {
  visitor.visit(*this);
}

}
