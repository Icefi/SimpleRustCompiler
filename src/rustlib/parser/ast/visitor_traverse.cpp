#include "ast.hpp"


namespace rust_lang::parser {

std::size_t VisitorTraverse::m_intent = 0;

VisitorTraverse::VisitorTraverse(std::ofstream& file) : m_stream(file) {}

void VisitorTraverse::increase_intent() {
  ++m_intent;
}
void VisitorTraverse::decrease_intent() {
  --m_intent;
}
std::size_t VisitorTraverse::get_intent() {
  return m_intent;
}

void VisitorTraverse::put_spaces() const {
  for (std::size_t i = 0; i < m_intent; i++) {
    m_stream << "  ";
  }
}

void VisitorTraverse::visit(ASTProgram& node) {
  m_stream << "<program>\n";

  for(auto& child: node.children()) {
    child->accept(*this);
  }

  m_stream << "</program>\n";
}

void VisitorTraverse::visit(ASTModule& node) {
  increase_intent();

  put_spaces();
  m_stream << "<module name=\"" << node.module_name() << "\"";

  if (node.size() == 0) {
    m_stream << "/>\n";
  } else {
    m_stream << ">\n";

    for (unsigned int i = 0; i < node.size(); i++) {
      node.expression(i)->accept(*this);
    }

    put_spaces();
    m_stream << "</module>\n";
  }

  decrease_intent();
}

void VisitorTraverse::visit(ASTIf& node) {
  increase_intent();
  put_spaces();

  m_stream << "<if>\n";

  increase_intent();
  put_spaces();
  m_stream << "<condition>\n";

  node.if_cond()->accept(*this);
  put_spaces();
  m_stream << "</condition>\n";
  decrease_intent();
  node.if_scope()->accept(*this);

  put_spaces();
  m_stream << "</if>\n";

  for (std::size_t i = 0; i < node.elif_conds().size(); i++) {
    put_spaces();
    m_stream << "<else-if>\n";

    increase_intent();
    put_spaces();
    m_stream << "<condition>\n";
    node.elif_cond(i)->accept(*this);
    put_spaces();
    m_stream << "</condition>\n";
    decrease_intent();
    node.elif_scope(i)->accept(*this);

    put_spaces();
    m_stream << "</else-if>\n";
  }

  if (node.else_scope() != nullptr) {
    put_spaces();
    m_stream << "<else>\n";

    node.else_scope()->accept(*this);

    put_spaces();
    m_stream << "</else>\n";
  }

  decrease_intent();
}

void VisitorTraverse::visit(ASTWhile& node) {
  increase_intent();
  put_spaces();

  m_stream << "<while>\n";

  increase_intent();
  put_spaces();
  m_stream << "<condition>\n";

  node.condition()->accept(*this);
  put_spaces();
  m_stream << "</condition>\n";

  node.scope()->accept(*this);

  decrease_intent();
  put_spaces();
  m_stream << "</while>\n";

  decrease_intent();
}

void VisitorTraverse::visit(ASTFor& node) {
  increase_intent();
  put_spaces();

  m_stream << "<for \"" << node.variable()->name() << "\">\n";

  increase_intent();

  put_spaces();
  m_stream << "<from>\n";

  node.first()->accept(*this);

  put_spaces();
  m_stream << "</from>\n";

  put_spaces();
  m_stream << "<to>\n";

  node.last()->accept(*this);

  put_spaces();
  m_stream << "</to>\n";

  decrease_intent();

  node.scope()->accept(*this);

  put_spaces();
  m_stream << "</for>\n";

  decrease_intent();
}

void VisitorTraverse::visit(ASTLoop& node) {
  increase_intent();

  put_spaces();
  m_stream << "<loop>\n";

  node.scope()->accept(*this);

  put_spaces();
  m_stream << "</loop>\n";

  decrease_intent();
}

void VisitorTraverse::visit(ASTStructDefinition& node) {
  increase_intent();

  put_spaces();
  m_stream << "<struct-definition name=\"" << node.struct_name() << "\"";

  if (node.size() == 0) {
    m_stream << "/>\n";
  } else {
    m_stream << "\n";

    for (unsigned int i = 0; i < node.size(); i++) {
      node.field(i)->accept(*this);
    }

    put_spaces();
    m_stream << "</struct-definition>\n";
  }

  decrease_intent();
}

void VisitorTraverse::visit(ASTStructImplementation& node) {
  increase_intent();

  put_spaces();
  m_stream << "<struct-implementation name=\"" << node.struct_name() << "\"";

  if (node.size() == 0) {
    m_stream << "/>\n";
  } else {
    m_stream << "\n";

    for (auto& method : node.methods()) {
      method->accept(*this);
    }

    put_spaces();
    m_stream << "</struct-implementation>\n";
  }

  decrease_intent();
}

void VisitorTraverse::visit(ASTTraitDefinition& node) {
  increase_intent();

  put_spaces();
  m_stream << "<trait-definition name=\"" << node.trait_name() << "\"";

  if (node.size() == 0) {
    m_stream << "/>\n";
  } else {
    m_stream << "\n";

    for (unsigned int i = 0; i < node.size(); i++) {
      node.func(i)->accept(*this);
    }

    put_spaces();
    m_stream << "</trait-definition>\n";
  }

  decrease_intent();
}

void VisitorTraverse::visit(ASTTraitImplementation& node) {
  increase_intent();

  put_spaces();
  m_stream << "<trait-implementation name=\"" << node.trait_name()
           << "\" for \"" << node.type_name() << "\"";

  if (node.size() == 0) {
    m_stream << "/>\n";
  } else {
    m_stream << "\n";

    for (auto& func : node.funcs()) {
      func->accept(*this);
    }

    put_spaces();
    m_stream << "</trait-implementation>\n";
  }

  decrease_intent();
}

void VisitorTraverse::visit(ASTScope& node) {
  for(auto& child: node.children()) {
    child->accept(*this);
  }
}

void VisitorTraverse::visit(ASTFunction& node) {
  increase_intent();

  put_spaces();
  m_stream << "<function name=\"" << node.func_name() << "\" return-type=\""
           << node.return_type() << "\"";

  if (node.size() == 0 && node.scope() == nullptr) {
    m_stream << "/>\n";
    decrease_intent();
    return;
  }

  m_stream << ">\n";

  if (node.size() != 0) {
    increase_intent();
    put_spaces();

    m_stream << "<args>\n";

    for (auto& arg : node.arguments()) {
      arg->accept(*this);
    }

    put_spaces();
    decrease_intent();

    m_stream << "</args>\n";
  }

  node.scope()->accept(*this);
  

  put_spaces();
  m_stream << "</function>\n";

  decrease_intent();
}

void VisitorTraverse::visit(ASTValue& node) {
  increase_intent();

  put_spaces();

  if (node.is_literal()) {
    m_stream << "<value \"" << node.name() << "\"/>\n";
  } else if (node.type() == "none") {
    m_stream << "<variable \"" << node.name() << "\"/>\n";
  } else {
    m_stream << "<variable \"" << node.name() << "\" type=\""
             << (node.is_mutable() ? ("mutable ") : ("constant "))
             << node.type() << "\"/>\n";
  }

  decrease_intent();
}

void VisitorTraverse::visit(ASTCall& node) {
  increase_intent();

  put_spaces();
  m_stream << "<call name=\"" << node.name() << "\"";

  if (node.size() == 0) {
    m_stream << "/>\n";
    decrease_intent();
    return;
  }

  m_stream << ">\n";

  increase_intent();
  put_spaces();

  m_stream << "<args>\n";

  for (auto& arg : node.arguments()) {
    arg->accept(*this);
  }

  put_spaces();
  m_stream << "</args>\n";

  decrease_intent();

  put_spaces();
  m_stream << "</call>\n";

  decrease_intent();
}

void VisitorTraverse::visit(ASTReturn& node) {
  increase_intent();

  if (node.value() == nullptr) {
    put_spaces();
    m_stream << "<return/>\n";
    decrease_intent();
    return;
  }

  put_spaces();
  m_stream << "<return>\n";

  node.value()->accept(*this);

  put_spaces();
  m_stream << "</return>\n";

  decrease_intent();
}

void VisitorTraverse::visit(ASTBreak& node) {
  increase_intent();

  if (node.value() == nullptr) {
    put_spaces();
    m_stream << "<break/>\n";
    decrease_intent();
    return;
  }

  put_spaces();
  m_stream << "<break>\n";

  node.value()->accept(*this);

  put_spaces();
  m_stream << "</return>\n";

  decrease_intent();
}

void VisitorTraverse::visit(ASTAssign& node) {
  increase_intent();

  ASTValue* lvalue_v = dynamic_cast<ASTValue*>(node.lvalue());
  ASTArrAccess* lvalue_a = dynamic_cast<ASTArrAccess*>(node.lvalue());

  put_spaces();
  
  if (lvalue_v != nullptr) {
    m_stream << "<assign \"" << lvalue_v->name() << "\">\n";
  } else {
    m_stream << "<assign \"" << lvalue_a->name() << "\">\n";

    lvalue_a->accept(*this);
  }

  node.rvalue()->accept(*this);

  put_spaces();
  m_stream << "</assign>\n";

  decrease_intent();
}

void VisitorTraverse::visit(ASTExpression& node) {
  /* Something like a + b */
  if (node.left() != nullptr && node.right() != nullptr) {
    increase_intent();
    put_spaces();
    m_stream << "<operation \"" << node.operation() << "\">\n";

    node.left()->accept(*this);
    node.right()->accept(*this);

    put_spaces();
    decrease_intent();
    m_stream << "</operation>\n";

    /* Operation in parens or an atom of the expression */
  } else {
    node.term()->accept(*this);
  }
}

void VisitorTraverse::visit(ASTAgrList& node) {
  increase_intent();

  put_spaces();
  m_stream << "<Agregate-list type=\"" << node.type() << "\">\n";

  for(auto& unit: node.units()) {
    unit->accept(*this);
  }

  put_spaces();
  m_stream << "</Agregate-list>\n";

  decrease_intent();
}

void VisitorTraverse::visit(ASTArrAccess& node) {
  increase_intent();
  
  put_spaces();
  m_stream << "<Array name=\"" << node.name() << "\" type=\"" << node.type() << "\">\n";

  node.expression()->accept(*this);

  put_spaces();
  m_stream << "</Array>\n";

  decrease_intent();
}

}
