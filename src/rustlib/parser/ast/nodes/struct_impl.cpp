#include "../ast.hpp"


namespace rust_lang::parser {

std::string ASTStructImplementation::struct_name() const {
  return m_struct_name;
}

std::vector<ASTFunction*>& ASTStructImplementation::methods() {
  return m_methods;
}

ASTFunction* ASTStructImplementation::method(std::size_t i) const {
  return m_methods[i];
}

std::size_t ASTStructImplementation::size() const {
  return m_methods.size();
}

void ASTStructImplementation::set_struct_name(std::string name) {
  m_struct_name = std::move(name);
}

void ASTStructImplementation::append_method(ASTFunction* method) {
  m_methods.push_back(method);
}

void ASTStructImplementation::accept(Visitor& visitor) {
  visitor.visit(*this);
}

}
