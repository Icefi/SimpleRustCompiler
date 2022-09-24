#include "../ast.hpp"


namespace rust_lang::parser {
  
void ASTBreak::accept(Visitor& visitor) {
  visitor.visit(*this);
}

}
