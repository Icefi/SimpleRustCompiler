#include "../ast.hpp"


namespace rust_lang::parser {

std::string ASTTraitImplementation::type_name() const {
  return m_type_name;
}
void ASTTraitImplementation::set_type_name(std::string type) {
  m_type_name = std::move(type);
}

void ASTTraitImplementation::accept(Visitor& visitor) {
  visitor.visit(*this);
}

}