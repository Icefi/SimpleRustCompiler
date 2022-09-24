#include "../ast.hpp"


namespace rust_lang::parser {

ASTArrAccess::~ASTArrAccess() { delete m_expr; }

std::string ASTArrAccess::name() const { return m_name; }
std::string ASTArrAccess::type() const { return m_type; }
std::size_t ASTArrAccess::num_elems() const { return m_num_elems; }
ASTExpression* ASTArrAccess::expression() const { return m_expr; }

void ASTArrAccess::set_name(const std::string& name) { m_name = name; }
void ASTArrAccess::set_type(const std::string& type) { m_type = type; }
void ASTArrAccess::set_expression(ASTExpression* node) { m_expr = node; }
void ASTArrAccess::set_num_elems(std::size_t num) { m_num_elems = num; }

void ASTArrAccess::accept(Visitor& visitor) { visitor.visit(*this); }


}  // rust_lang::parser
