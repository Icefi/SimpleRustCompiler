#include "../ast.hpp"


namespace rust_lang::parser {
  
ASTTraitDefinition::~ASTTraitDefinition() {
  for (unsigned int i = 0; i < size(); i++) {
    delete m_funcs[i];
  }
}

std::string ASTTraitDefinition::trait_name() const {
  return m_trait_name;
}
void ASTTraitDefinition::set_trait_name(std::string name) {
  m_trait_name = std::move(name);
}

ASTFunction* ASTTraitDefinition::func(std::size_t i) const {
  return m_funcs[i];
}

std::vector<ASTFunction*>& ASTTraitDefinition::funcs() {
  return m_funcs;
}

void ASTTraitDefinition::append_func(ASTFunction* func) {
  m_funcs.push_back(func);
}
std::size_t ASTTraitDefinition::size() const {
  return m_funcs.size();
}

void ASTTraitDefinition::accept(Visitor& visitor) {
  visitor.visit(*this);
}

}
