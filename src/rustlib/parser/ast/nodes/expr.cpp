#include "../ast.hpp"


namespace rust_lang::parser {
  
ASTExpression::~ASTExpression() {
  delete m_left;
  delete m_right;
}

std::string ASTExpression::operation() const {
  return m_operation;
}

std::string ASTExpression::type() const {
  return m_type;
}

ASTExpression* ASTExpression::left() const {
  return m_left;
}

ASTExpression* ASTExpression::right() const {
  return m_right;
}

ASTNode* ASTExpression::term() const {
  return m_term;
}

void ASTExpression::set_operation(const std::string& op) {
  m_operation = op;
}

void ASTExpression::set_type(const std::string& type) {
  m_type = type;
}

void ASTExpression::set_left(ASTExpression* left) {
  m_left = left;
}

void ASTExpression::set_right(ASTExpression* right) {
  m_right = right;
}

void ASTExpression::set_term(ASTNode* term) {
  m_term = term;
}

void ASTExpression::accept(Visitor& visitor) {
  visitor.visit(*this);
}

}