#include "../ast.hpp"


namespace rust_lang::parser {

ASTAgrList::~ASTAgrList() {
  for(auto& unit: m_units) {
    delete unit;
  }
}

std::vector<ASTExpression*>& ASTAgrList::units() {
  return m_units;
}

ASTExpression* ASTAgrList::unit(std::size_t i) const {
  return m_units[i];
}

void ASTAgrList::append_unit(ASTExpression* node) {
  m_units.push_back(node);
}

std::string ASTAgrList::type() const {
  return m_type;
}

void ASTAgrList::set_type(const std::string& type) {
  m_type = type;
}

std::size_t ASTAgrList::num_elems() const {
  return m_units.size();
}

void ASTAgrList::accept(Visitor& visitor) {
  visitor.visit(*this);
}


}  // rust_lang::parser
