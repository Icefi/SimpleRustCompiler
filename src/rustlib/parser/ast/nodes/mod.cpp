#include "../ast.hpp"


namespace rust_lang::parser {

ASTModule::~ASTModule() {
  for (unsigned int i = 0; i < size(); i++) {
    delete m_expressions[i];
  }
}

std::string ASTModule::module_name() const {
  return m_module_name;
}
void ASTModule::set_module_name(std::string name) {
  m_module_name = std::move(name);
}

void ASTModule::append_expression(ASTNode* node) {
  m_expressions.push_back(node);
}
ASTNode* ASTModule::expression(std::size_t i) const {
  return m_expressions[i];
}

std::vector<ASTNode*>& ASTModule::expressions() {
  return m_expressions;
}

std::size_t ASTModule::size() const {
  return m_expressions.size();
}

void ASTModule::accept(Visitor& visitor) {
  visitor.visit(*this);
}

}
