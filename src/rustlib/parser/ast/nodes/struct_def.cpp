#include "../ast.hpp"


namespace rust_lang::parser {

ASTStructDefinition::~ASTStructDefinition() {
  for (unsigned int i = 0; i < size(); i++) {
    delete m_fields[i];
  }
}


std::string ASTStructDefinition::struct_name() const {
  return m_struct_name;
}

std::vector<ASTValue*>& ASTStructDefinition::fields() {
  return m_fields;
}

ASTValue* ASTStructDefinition::field(std::size_t i) const {
  return m_fields[i];
}

std::size_t ASTStructDefinition::size() const {
  return m_fields.size();
}

void ASTStructDefinition::set_struct_name(std::string name) {
  m_struct_name = std::move(name);
}

void ASTStructDefinition::append_field(ASTValue* field) {
  m_fields.push_back(field);
}

void ASTStructDefinition::accept(Visitor& visitor) {
  visitor.visit(*this);
}

}
