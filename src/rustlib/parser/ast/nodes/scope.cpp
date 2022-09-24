#include "../ast.hpp"


namespace rust_lang::parser {

void ASTScope::accept(Visitor& visitor) {
  visitor.visit(*this);
}

}
