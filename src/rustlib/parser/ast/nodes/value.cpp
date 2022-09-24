#include "../ast.hpp"

namespace rust_lang::parser {

std::string ASTValue::name() const {
  return m_var_name;
}
std::string ASTValue::type() const {
  return m_var_type;
}

void ASTValue::set_name(std::string name) {
  m_var_name = std::move(name);
}

void ASTValue::set_type(std::string type) {
  m_var_type = std::move(type);
}

bool ASTValue::is_mutable() const {
  return m_is_mutable;
}
void ASTValue::set_mutable(bool mut) {
  m_is_mutable = mut;
}

bool ASTValue::is_definition() const {
  return m_is_definition;
}

void ASTValue::set_definition(bool def) {
  m_is_definition = def;
}

bool ASTValue::is_literal() const {
  return m_is_literal;
}

void ASTValue::set_literal(bool literal) {
  m_is_literal = literal;
}

std::size_t ASTValue::dimension() const {
  return m_dimension;
}

void ASTValue::set_dimension(std::size_t dim) {
  m_dimension = dim;
}

std::size_t ASTValue::num_elems() const {
  return m_num_elems;
}

void ASTValue::set_num_elems(std::size_t num) {
  m_num_elems = num;
}

void ASTValue::accept(Visitor& visitor) {
  visitor.visit(*this);
}

}
