#include "ast.hpp"

namespace rust_lang::parser {

void VisitorInitialiser::set_context(antlrcpp::Any context) {
  m_context = std::move(context);
}

VisitorInitialiser::VisitorInitialiser(antlrcpp::Any context)
  : m_context(std::move(context)) {}

SymbolTable VisitorInitialiser::m_table = SymbolTable();

SymbolTable VisitorInitialiser::table() { return std::move(m_table); }

Scope VisitorInitialiser::m_scope = Scope();

Scope VisitorInitialiser::scope() { return m_scope; }

void VisitorInitialiser::increase_level(const std::string& scope) { m_scope.push_back(scope); }

void VisitorInitialiser::decrease_level() { m_scope.pop_back(); }

StreamErrorListener* VisitorInitialiser::m_error_listener = nullptr;

void VisitorInitialiser::set_error_listener(StreamErrorListener* listener) {
  m_error_listener = listener;

  m_table.set_error_listener(listener);
}

void VisitorInitialiser::visit(ASTProgram& node) {
  increase_level("global");

  auto* ctx = m_context.as<RustLangParser::ProgramContext*>();

  ASTNode* child = nullptr;

  for (auto& expression : ctx->expressions()) {
    auto expr = visitExpressions(expression);

    if (expr.isNotNull()) {
      VisitorInitialiser visitor;

      if (expr.is<RustLangParser::Assign_stmtContext*>()) {
        visitor.set_context(expr.as<RustLangParser::Assign_stmtContext*>());

        child = new ASTAssign;
      } else if (expr.is<RustLangParser::Func_defContext*>()) {
        visitor.set_context(expr.as<RustLangParser::Func_defContext*>());

        child = new ASTFunction;
      } else if (expr.is<RustLangParser::Var_defContext*>()) {
        visitor.set_context(expr.as<RustLangParser::Var_defContext*>());

        child = new ASTValue;
      } else if (expr.is<RustLangParser::Mod_defContext*>()) {
        visitor.set_context(expr.as<RustLangParser::Mod_defContext*>());

        child = new ASTModule;
      } else if (expr.is<RustLangParser::Struct_defContext*>()) {
        visitor.set_context(expr.as<RustLangParser::Struct_defContext*>());

        child = new ASTStructDefinition;
      } else if (expr.is<RustLangParser::Struct_implContext*>()) {
        visitor.set_context(expr.as<RustLangParser::Struct_implContext*>());

        child = new ASTStructImplementation;
      } else if (expr.is<RustLangParser::Trait_defContext*>()) {
        visitor.set_context(expr.as<RustLangParser::Trait_defContext*>());

        child = new ASTTraitDefinition;
      } else if (expr.is<RustLangParser::Trait_implContext*>()) {
        visitor.set_context(expr.as<RustLangParser::Trait_implContext*>());

        child = new ASTTraitImplementation;
      } else {
        std::cerr << "Error: unexpected expression\n";
        return;
      }

      child->accept(visitor);

      node.append_child(child);
    }
  }

  decrease_level();
}

void VisitorInitialiser::visit(ASTModule& node) {
  auto* ctx = m_context.as<RustLangParser::Mod_defContext*>();

  node.set_module_name(ctx->ID()->getText());

  increase_level(node.module_name());

  ASTNode* child = nullptr;
  VisitorInitialiser visitor;

  for (auto& expression : ctx->expressions()) {
    auto expr = visitExpressions(expression);

    if (expr.isNotNull()) {
      if (expr.is<RustLangParser::Assign_stmtContext*>()) {
        visitor.set_context(expr.as<RustLangParser::Assign_stmtContext*>());

        child = new ASTAssign;
      } else if (expr.is<RustLangParser::Func_defContext*>()) {
        visitor.set_context(expr.as<RustLangParser::Func_defContext*>());

        child = new ASTFunction;
      } else if (expr.is<RustLangParser::Var_defContext*>()) {
        visitor.set_context(expr.as<RustLangParser::Var_defContext*>());

        child = new ASTValue;
      } else if (expr.is<RustLangParser::Mod_defContext*>()) {
        visitor.set_context(expr.as<RustLangParser::Mod_defContext*>());

        child = new ASTModule;
      } else if (expr.is<RustLangParser::Struct_defContext*>()) {
        visitor.set_context(expr.as<RustLangParser::Struct_defContext*>());

        child = new ASTStructDefinition;
      } else if (expr.is<RustLangParser::Struct_implContext*>()) {
        visitor.set_context(expr.as<RustLangParser::Struct_implContext*>());

        child = new ASTStructImplementation;
      } else if (expr.is<RustLangParser::Trait_defContext*>()) {
        visitor.set_context(expr.as<RustLangParser::Trait_defContext*>());

        child = new ASTTraitDefinition;
      } else if (expr.is<RustLangParser::Trait_implContext*>()) {
        visitor.set_context(expr.as<RustLangParser::Trait_implContext*>());

        child = new ASTTraitImplementation;
      } else {
        std::cerr << "Error: unexpected expression\n";
        return;
      }

      child->accept(visitor);

      node.append_expression(child);
    }
  }

  m_table.insert(
    node.module_name(),
    SymbolKind::MODULE,
    UsageType::DEFINITION,
    "", 1, 1,
    ctx->ID()->getSymbol()->getLine(),
    ctx->ID()->getSymbol()->getCharPositionInLine(),
    m_scope
  );

  decrease_level();
}

void VisitorInitialiser::visit(ASTScope& node) {
  auto* ctx = m_context.as<RustLangParser::ScopeContext*>();

  ASTNode* child = nullptr;

  auto num_scopes = 0;

  for (auto& statement : ctx->statements()) {
    auto stmt = visitStatements(statement);

    if (stmt.isNotNull()) {
      VisitorInitialiser visitor;

      if (stmt.is<RustLangParser::ScopeContext*>()) {

        visitor.set_context(stmt.as<RustLangParser::ScopeContext*>());

        child = new ASTScope;

        increase_level(std::to_string(++num_scopes));
        child->accept(visitor);
        decrease_level();

        node.append_child(child);

      } else if (stmt.is<RustLangParser::Return_stmtContext*>()) {
        visitor.set_context(stmt.as<RustLangParser::Return_stmtContext*>());

        child = new ASTReturn;

        child->accept(visitor);

        node.append_child(child);
      } else if (stmt.is<RustLangParser::Break_stmtContext*>()) {
        visitor.set_context(stmt.as<RustLangParser::Break_stmtContext*>());

        child = new ASTBreak;

        child->accept(visitor);

        node.append_child(child);
      } else if (stmt.is<RustLangParser::Loop_stmtContext*>()) {
        visitor.set_context(stmt.as<RustLangParser::Loop_stmtContext*>());

        child = new ASTLoop;

        increase_level(std::to_string(++num_scopes));
        child->accept(visitor);
        decrease_level();

        node.append_child(child);
      } else if (stmt.is<RustLangParser::If_stmtContext*>()) {
        visitor.set_context(stmt.as<RustLangParser::If_stmtContext*>());

        child = new ASTIf;

        increase_level(std::to_string(++num_scopes));
        child->accept(visitor);
        decrease_level();


        node.append_child(child);

      } else if (stmt.is<RustLangParser::For_stmtContext*>()) {
        visitor.set_context(stmt.as<RustLangParser::For_stmtContext*>());

        increase_level(std::to_string(++num_scopes));
        child = new ASTFor;
        decrease_level();

        child->accept(visitor);


        node.append_child(child);
      } else if (stmt.is<RustLangParser::While_stmtContext*>()) {
        visitor.set_context(stmt.as<RustLangParser::While_stmtContext*>());

        child = new ASTWhile;

        increase_level(std::to_string(++num_scopes));
        child->accept(visitor);
        decrease_level();
        
        node.append_child(child);
      } else if (stmt.is<RustLangParser::Func_callContext*>()) {
        visitor.set_context(stmt.as<RustLangParser::Func_callContext*>());

        child = new ASTCall;

        child->accept(visitor);

        node.append_child(child);
      } else if (stmt.is<RustLangParser::Assign_stmtContext*>()) {
        visitor.set_context(stmt.as<RustLangParser::Assign_stmtContext*>());

        child = new ASTAssign;

        child->accept(visitor);

        node.append_child(child);
      } else if (stmt.is<RustLangParser::Var_defContext*>()) {
        visitor.set_context(stmt.as<RustLangParser::Var_defContext*>());

        child = new ASTValue;

        child->accept(visitor);

        node.append_child(child);
      } else {
        std::cerr << "Error: unexpected statement\n";
        return;
      }
    }
  }
}

void VisitorInitialiser::visit(ASTIf& node) {
  auto* ctx = m_context.as<RustLangParser::If_stmtContext*>();

  auto scopes = ctx->scope();
  auto conds = ctx->expr_op();

  VisitorInitialiser visitor;

  auto* if_cond = new ASTExpression;
  auto* if_scope = new ASTScope;

  visitor.set_context(conds[0]);
  if_cond->accept(visitor);
  node.set_if_cond(if_cond);

  increase_level("if");
  visitor.set_context(scopes[0]);
  if_scope->accept(visitor);
  node.set_if_scope(if_scope);
  decrease_level();

  for (std::size_t i = 1; i < conds.size(); i++) {
    auto* cond = new ASTExpression;
    auto* scope = new ASTScope;

    visitor.set_context(conds[i]);
    cond->accept(visitor);

    visitor.set_context(scopes[i]);

    increase_level("elif" + std::to_string(i));
    scope->accept(visitor);
    decrease_level();
    
    node.append_cond(cond);
    node.append_scope(scope);
  }

  if (scopes.size() > conds.size()) {
    auto* child = new ASTScope;

    visitor.set_context(scopes.back());

    increase_level("else");
    child->accept(visitor);
    decrease_level();

    node.set_else_scope(child);
  }
}

void VisitorInitialiser::visit(ASTWhile& node) {
  increase_level("while");

  auto* ctx = m_context.as<RustLangParser::While_stmtContext*>();

  auto* condition = new ASTExpression;
  auto* scope = new ASTScope;

  VisitorInitialiser visitor;

  visitor.set_context(ctx->expr_op());
  condition->accept(visitor);

  visitor.set_context(ctx->scope());
  scope->accept(visitor);

  node.set_condition(condition);
  node.set_scope(scope);

  decrease_level();
}

void VisitorInitialiser::visit(ASTFor& node) {
  increase_level("for");

  VisitorInitialiser visitor;

  auto* ctx = m_context.as<RustLangParser::For_stmtContext*>();

  /* ITERATED VARIABLE */

  auto* var = new ASTValue;

  var->set_literal(false);
  var->set_name(ctx->ID()->getText());

  node.set_variable(var);

  /* FIRST ITERATOR'S POSITION */

  auto* first = new ASTExpression;

  visitor.set_context(ctx->expr_op(0));
  first->accept(visitor);

  node.set_first(first);

  /* LAST ITERATOR'S POSITION */

  auto* last = new ASTExpression;

  visitor.set_context(ctx->expr_op(1));
  last->accept(visitor);

  node.set_last(last);

  /* SCOPE */

  auto* scope = new ASTScope;

  visitor.set_context(ctx->scope());
  scope->accept(visitor);

  node.set_scope(scope);

  decrease_level();
}

void VisitorInitialiser::visit(ASTLoop& node) {
  increase_level("loop");

  auto* ctx = m_context.as<RustLangParser::Loop_stmtContext*>();
  auto* scope = new ASTScope;

  VisitorInitialiser visitor(ctx->scope());

  scope->accept(visitor);

  node.set_scope(scope);

  decrease_level();
}

void VisitorInitialiser::visit(ASTStructDefinition& node) {
  auto* ctx = m_context.as<RustLangParser::Struct_defContext*>();

  node.set_struct_name(ctx->ID()->getText());

  increase_level(node.struct_name());

  for (std::size_t i = 0; i < ctx->var_typedec().size(); i++) {
    auto* field = new ASTValue;

    field->set_name(ctx->var_typedec(i)->ID(0)->getText());
    field->set_type(ctx->var_typedec(i)->ID(1)->getText());

    node.append_field(field);
  }

  m_table.insert(
    node.struct_name(),
    SymbolKind::STRUCT,
    UsageType::DEFINITION,
    node.struct_name(),
    1, 1,
    ctx->ID()->getSymbol()->getLine(),
    ctx->ID()->getSymbol()->getCharPositionInLine(),
    m_scope
  );

  decrease_level();
}

void VisitorInitialiser::visit(ASTStructImplementation& node) {
  auto* ctx = m_context.as<RustLangParser::Struct_implContext*>();

  node.set_struct_name(ctx->ID()->getText());

  increase_level(node.struct_name());

  for (std::size_t i = 0; i < ctx->expressions().size(); i++) {
    auto* method = new ASTFunction;

    auto expr = visitExpressions(ctx->expressions(i));

    VisitorInitialiser visitor(expr.as<RustLangParser::Func_defContext*>());
    method->accept(visitor);

    node.append_method(method);
  }

  m_table.insert(
    node.struct_name(),
    SymbolKind::STRUCT,
    UsageType::IMPLEMENTATION,
    node.struct_name(),
    1, 1,
    ctx->ID()->getSymbol()->getLine(),
    ctx->ID()->getSymbol()->getCharPositionInLine(),
    m_scope
  );

  decrease_level();
}

void VisitorInitialiser::visit(ASTTraitDefinition& node) {
  auto* ctx = m_context.as<RustLangParser::Trait_defContext*>();

  node.set_trait_name(ctx->ID()->getText());

  increase_level(node.trait_name());

  for (std::size_t i = 0; i < ctx->expressions().size(); i++) {
    auto* func = new ASTFunction;

    auto expr = visitExpressions(ctx->expressions(i));
    VisitorInitialiser visitor(expr.as<RustLangParser::Func_defContext*>());

    func->accept(visitor);

    node.append_func(func);
  }

  m_table.insert(
    node.trait_name(),
    SymbolKind::TRAIT,
    UsageType::DEFINITION,
    "", 1, 1,
    ctx->ID()->getSymbol()->getLine(),
    ctx->ID()->getSymbol()->getCharPositionInLine(),
    m_scope
  );

  decrease_level();
}

void VisitorInitialiser::visit(ASTTraitImplementation& node) {
  auto* ctx = m_context.as<RustLangParser::Trait_implContext*>();

  node.set_trait_name(ctx->ID(0)->getText());
  node.set_type_name(ctx->ID(1)->getText());

  increase_level(node.trait_name());

  for (std::size_t i = 0; i < ctx->expressions().size(); i++) {
    auto* func = new ASTFunction;

    auto expr = visitExpressions(ctx->expressions(i));
    VisitorInitialiser visitor(expr.as<RustLangParser::Func_defContext*>());

    func->accept(visitor);

    node.append_func(func);
  }

  m_table.insert(
    node.trait_name(),
    SymbolKind::TRAIT,
    UsageType::IMPLEMENTATION,
    node.type_name(),
    1, 1,
    ctx->ID(0)->getSymbol()->getLine(),
    ctx->ID(0)->getSymbol()->getCharPositionInLine(),
    m_scope
  );

  decrease_level();
}

void VisitorInitialiser::visit(ASTFunction& node) {
  auto* ctx = m_context.as<RustLangParser::Func_defContext*>();

  node.set_func_name(ctx->ID(0)->getText());

  node.set_return_type(ctx->ID(1) != nullptr ? ctx->ID(1)->getText() : "void" );

  increase_level(node.func_name());

  for (auto& argument : ctx->var_typedec()) {
    auto* arg = new ASTValue;

    arg->set_name(argument->ID(0)->getText());
    arg->set_type(argument->ID(1)->getText());
    arg->set_literal(false);

    node.append_argument(arg);

    m_table.insert(
      arg->name(),
      SymbolKind::VARIABLE,
      UsageType::DEFINITION,
      arg->type(),
      1, 1,
      argument->ID(0)->getSymbol()->getLine(),
      argument->ID(0)->getSymbol()->getCharPositionInLine(),
      m_scope
    );
  }

  VisitorInitialiser visitor(ctx->scope());

  auto* child = new ASTScope;

  child->accept(visitor);

  node.set_scope(child);

  m_table.insert(
    node.func_name(),
    SymbolKind::FUNCTION,
    UsageType::DEFINITION,
    node.return_type(),
    1, 1,
    ctx->ID(0)->getSymbol()->getLine(),
    ctx->ID(0)->getSymbol()->getCharPositionInLine(),
    m_scope
  );

  decrease_level();
}

void VisitorInitialiser::visit(ASTValue& node) {
  auto* ctx = m_context.as<RustLangParser::Var_defContext*>();

  node.set_mutable(ctx->MUT() != nullptr);

  node.set_name(ctx->ID(0)->getText());
  node.set_type(ctx->ID(1) == nullptr ? "auto" : ctx->ID(1)->getText());
  
  node.set_literal(false);

  node.set_definition(true);

  node.set_dimension(1 + (ctx->LSBR() != nullptr));

  m_table.insert(
    node.name(),
    SymbolKind::VARIABLE,
    UsageType::DEFINITION,
    node.type(),
    node.dimension(),
    node.num_elems(),
    ctx->ID(0)->getSymbol()->getLine(),
    ctx->ID(0)->getSymbol()->getCharPositionInLine(),
    m_scope
  );
}

void VisitorInitialiser::visit(ASTCall& node) {
  auto* ctx = m_context.as<RustLangParser::Func_callContext*>();

  node.set_name(ctx->ID()->getText());

  VisitorInitialiser visitor;

  for (auto& arg : ctx->expr_op()) {
    auto* child = new ASTExpression;

    visitor.set_context(arg);
    child->accept(visitor);

    node.append_argument(child);
  }

  if (node.name() == "print") {
    node.set_name("printf");
    node.set_type("i32 (i8*, ...)");

    if (node.size() == 0) {
      m_error_listener->semanticError(
        ctx->ID()->getSymbol()->getLine(),
        ctx->ID()->getSymbol()->getCharPositionInLine(),
        "mismatched types: first argument must be str, but nothing were provided"
      );

      return;
    }

    m_table.insert(
      "print",
      SymbolKind::FUNCTION,
      UsageType::CALL,
      "i32", 1, 1,
      ctx->ID()->getSymbol()->getLine(),
      ctx->ID()->getSymbol()->getCharPositionInLine(),
      m_scope
    );

    return;
  }
  
  if (node.name() == "scan") {

    node.set_name("__isoc99_scanf");
    node.set_type("i32 (i8*, ...)");

    if (node.size() == 0) {
      m_error_listener->semanticError(
        ctx->ID()->getSymbol()->getLine(),
        ctx->ID()->getSymbol()->getCharPositionInLine(),
        "mismatched types: first argument must be str, but nothing were provided"
      );

      return;
    }

    m_table.insert(
      "scan",
      SymbolKind::FUNCTION,
      UsageType::CALL,
      "i32", 1, 1,
      ctx->ID()->getSymbol()->getLine(),
      ctx->ID()->getSymbol()->getCharPositionInLine(),
      m_scope
    );

    return;
  }

  auto* tmp = m_table.insert(
    node.name(),
    SymbolKind::FUNCTION,
    UsageType::CALL,
    "auto",
    1, 1,
    ctx->ID()->getSymbol()->getLine(),
    ctx->ID()->getSymbol()->getCharPositionInLine(),
    m_scope
  );

  if (tmp == nullptr) { return; }
  
  node.set_type(tmp->m_type);
  
}

void VisitorInitialiser::visit(ASTReturn& node) {
  auto* ctx = m_context.as<RustLangParser::Return_stmtContext*>();

  if (ctx->expr_op() == nullptr) {
    return;
  }

  VisitorInitialiser visitor(ctx->expr_op());
  auto* return_value = new ASTExpression;

  return_value->accept(visitor);

  node.set_value(return_value);
}

void VisitorInitialiser::visit(ASTBreak& node) {
  auto* ctx = m_context.as<RustLangParser::Break_stmtContext*>();

  if (ctx->expr_op() == nullptr) {
    return;
  }

  VisitorInitialiser visitor(ctx->expr_op());
  auto* return_value = new ASTExpression;

  return_value->accept(visitor);

  node.set_value(return_value);
}

void VisitorInitialiser::visit(ASTAssign& node) {
  auto* ctx = m_context.as<RustLangParser::Assign_stmtContext*>();

  /* RVALUE */

  VisitorInitialiser visitor;

  ASTNode* rvalue = nullptr;

  if (ctx->expr_op() != nullptr) {
    rvalue = new ASTExpression;
    visitor.set_context(ctx->expr_op());
  } else {
    rvalue = new ASTAgrList;
    visitor.set_context(ctx->agr_list());
  }

  rvalue->accept(visitor);

  node.set_rvalue(rvalue);

  ASTAgrList* rvalue_as_agr = dynamic_cast<ASTAgrList*>(rvalue);
  ASTExpression* rvalue_as_expr = dynamic_cast<ASTExpression*>(rvalue);

  /* LVALUE */

  if (ctx->ID() != nullptr) {
    // assignment

    auto* lvalue = new ASTValue;

    lvalue->set_name(ctx->ID()->getText());
    lvalue->set_literal(false);

    auto* tmp = m_table.insert(
      lvalue->name(),
      SymbolKind::VARIABLE,
      UsageType::ASSIGNMENT,
      "auto", 0, 0,
      ctx->ID()->getSymbol()->getLine(),
      ctx->ID()->getSymbol()->getCharPositionInLine(),
      m_scope
    );

    if (tmp == nullptr) { return; }

    lvalue->set_type(tmp->m_type);
    lvalue->set_dimension(tmp->m_dimension);
    lvalue->set_num_elems(tmp->m_num_elems); 

    if (lvalue->dimension() == 2 && rvalue_as_agr == nullptr) {
      m_error_listener->semanticError(
        ctx->ASSIGN()->getSymbol()->getLine(),
        ctx->ASSIGN()->getSymbol()->getCharPositionInLine(),
        "Cannot cast a one-dimensional value to a two-dimensional");

      return;
    }

    if (rvalue_as_agr != nullptr && lvalue->num_elems() != rvalue_as_agr->num_elems()) {
      m_error_listener->semanticError(
        ctx->ASSIGN()->getSymbol()->getLine(),
        ctx->ASSIGN()->getSymbol()->getCharPositionInLine(),
        "Cannot cast an array of "
          + std::to_string(rvalue_as_agr->num_elems())
          + " to an array of "
          + std::to_string(lvalue->num_elems()));

      return;
    }

    if (rvalue_as_agr != nullptr && lvalue->type() != rvalue_as_agr->type()) {
      m_error_listener->semanticError(
        ctx->ASSIGN()->getSymbol()->getLine(),
        ctx->ASSIGN()->getSymbol()->getCharPositionInLine(),
        "Mismatched types");

      return;
    } else if (rvalue_as_expr != nullptr &&  lvalue->type() != rvalue_as_expr->type()) {
      m_error_listener->semanticError(
        ctx->ASSIGN()->getSymbol()->getLine(),
        ctx->ASSIGN()->getSymbol()->getCharPositionInLine(),
        "Mismatched types");

      return;
    }

    node.set_lvalue(lvalue);

  } else if (ctx->var_def() != nullptr) {
    // definition

    auto* lvalue = new ASTValue;

    lvalue->set_name(ctx->var_def()->ID(0)->getText());
    lvalue->set_type(ctx->var_def()->ID(1) == nullptr ? "auto" : ctx->var_def()->ID(1)->getText());
    lvalue->set_literal(false);
    lvalue->set_definition(true);

    if (ctx->var_def()->INTEGER() != nullptr) {
      lvalue->set_dimension(2);
      lvalue->set_num_elems(std::stoi(ctx->var_def()->INTEGER()->getText()));
    }

    if (rvalue_as_agr != nullptr) {
      if (lvalue->dimension() == 0) {
        lvalue->set_dimension(2);
      }

      if (lvalue->num_elems() == 0) {
        lvalue->set_num_elems(rvalue_as_agr->num_elems());
      }

      if (lvalue->type() == "auto") {
        lvalue->set_type(rvalue_as_agr->type());
      }

      if (lvalue->dimension() == 1) {
        m_error_listener->semanticError(
          ctx->ASSIGN()->getSymbol()->getLine(),
          ctx->ASSIGN()->getSymbol()->getCharPositionInLine(),
          "Mismatched dimensions"
        );

        return;
      }

      if (lvalue->num_elems() != rvalue_as_agr->num_elems()) {
        m_error_listener->semanticError(
          ctx->ASSIGN()->getSymbol()->getLine(),
          ctx->ASSIGN()->getSymbol()->getCharPositionInLine(),
          "Mismatched number of elements " + lvalue->name() + " and rvalue"
        );

        return;
      }

      if(lvalue->type() != rvalue_as_agr->type()) {
        m_error_listener->semanticError(
          ctx->ASSIGN()->getSymbol()->getLine(),
          ctx->ASSIGN()->getSymbol()->getCharPositionInLine(),
          "mismatched types: " + lvalue->type() + " and " + rvalue_as_agr->type()
        );
      }

    } else if (rvalue_as_expr != nullptr) {
      if (lvalue->num_elems() == 0) {
        lvalue->set_num_elems(1);
      }

      if (lvalue->dimension() == 0) {
        lvalue->set_dimension(1);
      }

      if (lvalue->type() == "auto") {
        lvalue->set_type(rvalue_as_expr->type());
      }

      if (lvalue->dimension() == 2) {
        m_error_listener->semanticError(
          ctx->ASSIGN()->getSymbol()->getLine(),
          ctx->ASSIGN()->getSymbol()->getCharPositionInLine(),
          "Mismatched dimensions"
        );

        return;
      }

      if (lvalue->num_elems() != 1) {
          m_error_listener->semanticError(
          ctx->ASSIGN()->getSymbol()->getLine(),
          ctx->ASSIGN()->getSymbol()->getCharPositionInLine(),
          "Cannot cast one-dimensional value to two-dimensional"
        );

        return;
      }

      if(lvalue->type() != rvalue_as_expr->type()) {
        m_error_listener->semanticError(
          ctx->ASSIGN()->getSymbol()->getLine(),
          ctx->ASSIGN()->getSymbol()->getCharPositionInLine(),
          "mismatched types: " + lvalue->type() + " and " + rvalue_as_expr->type()
        );

        return;
      }
    }

    m_table.insert(
      lvalue->name(),
      SymbolKind::VARIABLE,
      UsageType::DEFINITION,
      lvalue->type(),
      lvalue->dimension(),
      lvalue->num_elems(),
      ctx->var_def()->ID(0)->getSymbol()->getLine(),
      ctx->var_def()->ID(0)->getSymbol()->getCharPositionInLine(),
      m_scope
    );

    node.set_lvalue(lvalue);

  } else {
    // array access
    auto* lvalue = new ASTArrAccess;

    visitor.set_context(ctx->arr_access());

    lvalue->accept(visitor);

    if (rvalue_as_agr != nullptr) {
      m_error_listener->semanticError(
        ctx->ASSIGN()->getSymbol()->getLine(),
        ctx->ASSIGN()->getSymbol()->getCharPositionInLine(),
        "Cannot cast two-dimensional type to one-dimensional"
      );

      return;
    }

    if (rvalue_as_expr != nullptr && rvalue_as_expr->type() != lvalue->type()) {
      m_error_listener->semanticError(
        ctx->ASSIGN()->getSymbol()->getLine(),
        ctx->ASSIGN()->getSymbol()->getCharPositionInLine(),
        "Mismatched types"
      );

      return;
    }

    node.set_lvalue(lvalue);
  }
}

void VisitorInitialiser::visit(ASTExpression& node) {
  auto* ctx = m_context.as<RustLangParser::Expr_opContext*>();

  VisitorInitialiser visitor;

  auto expressions = ctx->expr_op();

  if (expressions.empty()) {
    if (ctx->unit()->ID() != nullptr) {
      auto* term = new ASTValue;

      term->set_name(ctx->unit()->ID()->getText());
      term->set_literal(false);

      node.set_term(term);

      auto* tmp = m_table.insert(
        term->name(),
        SymbolKind::VARIABLE,
        UsageType::GET,
        "auto", 1, 1,
        ctx->unit()->ID()->getSymbol()->getLine(),
        ctx->unit()->ID()->getSymbol()->getCharPositionInLine(),
        m_scope
      );

      if (tmp == nullptr) { return; }

      if (term->type() == "auto") { term->set_type(tmp->m_type); }
      if (term->dimension() == 0) { term->set_dimension(tmp->m_dimension); }
      if (term->num_elems() == 0) { term->set_num_elems(tmp->m_num_elems); }

      node.set_type(term->type());

    } else if (ctx->unit()->INTEGER() != nullptr) {
      auto* term = new ASTValue;

      term->set_name(ctx->unit()->INTEGER()->getText());
      term->set_type("i32");
      term->set_literal(true);

      node.set_term(term);
      node.set_type(term->type());
    } else if (ctx->unit()->FLOAT() != nullptr) {
      auto* term = new ASTValue;

      term->set_name(ctx->unit()->FLOAT()->getText());
      term->set_type("f32");
      term->set_literal(true);

      node.set_term(term);
      node.set_type(term->type());
    } else if (ctx->unit()->STRING() != nullptr) {
      auto* term = new ASTValue;

      std::string name = ctx->unit()->STRING()->getText();
      name.pop_back();
      name.erase(0, 1);

      term->set_name(name);
      term->set_type("str");
      term->set_literal(true);

      node.set_term(term);
      node.set_type(term->type());
    } else if (ctx->unit()->func_call() != nullptr) {
      auto* term = new ASTCall;

      visitor.set_context(ctx->unit()->func_call());
      term->accept(visitor);

      node.set_term(term);

      auto* tmp = m_table.insert(
        term->name(),
        SymbolKind::FUNCTION,
        UsageType::CALL,
        "auto", 1, 1,
        ctx->unit()->func_call()->ID()->getSymbol()->getLine(),
        ctx->unit()->func_call()->ID()->getSymbol()->getCharPositionInLine(),
        m_scope
      );

      if (tmp == nullptr) { return; }

      term->set_type(tmp->m_type);

      node.set_type(term->type());

    } else if (ctx->unit()->arr_access() != nullptr) {
      auto* term = new ASTArrAccess;

      visitor.set_context(ctx->unit()->arr_access());
      term->accept(visitor);

      node.set_term(term);
      node.set_type(term->type());
    }

    if (ctx->PLUS() != nullptr) {
      node.set_operation("+");
    } else if (ctx->MINUS() != nullptr) {
      node.set_operation("-");
      ((ASTValue*)node.term())->set_name("-" + ((ASTValue*)node.term())->name());
    }

  } else if (expressions.size() == 1) {
    visitor.set_context(ctx->expr_op(0));

    auto* expr = new ASTExpression;
    expr->accept(visitor);

    node.set_term(expr);
    node.set_operation("()");
    node.set_type(expr->type());

  } else {
    auto* left = new ASTExpression;
    auto* right = new ASTExpression;

    visitor.set_context(ctx->expr_op(0));
    left->accept(visitor);
    node.set_left(left);

    visitor.set_context(ctx->expr_op(1));
    right->accept(visitor);
    node.set_right(right);

    antlr4::tree::TerminalNode *op_ptr = nullptr;

    if (ctx->STAR() != nullptr) {

      op_ptr = ctx->STAR();
      node.set_operation("*");
    } else if (ctx->SLASH() != nullptr) {

      op_ptr = ctx->SLASH();
      node.set_operation("/");
    } else if (ctx->PERCNT() != nullptr) {

      op_ptr = ctx->PERCNT();
      node.set_operation("%");
    } else if (ctx->PLUS() != nullptr) {

      op_ptr = ctx->PLUS();
      node.set_operation("+");
    } else if (ctx->MINUS() != nullptr) {

      op_ptr = ctx->MINUS();
      node.set_operation("-");
    } else if (ctx->LSHIFT() != nullptr) {

      op_ptr = ctx->LSHIFT();
      node.set_operation("<<");
    } else if (ctx->RSHIFT() != nullptr) {

      op_ptr = ctx->RSHIFT();
      node.set_operation(">>");
    } else if (ctx->LESS() != nullptr) {

      op_ptr = ctx->LESS();
      node.set_operation("<");
      node.set_type("bool");
    } else if (ctx->LESSEQ() != nullptr) {

      op_ptr = ctx->LESSEQ();
      node.set_operation("<=");
      node.set_type("bool");
    } else if (ctx->GR() != nullptr) {

      op_ptr = ctx->GR();
      node.set_operation(">");
      node.set_type("bool");
    } else if (ctx->GREQ() != nullptr) {

      op_ptr = ctx->GREQ();
      node.set_operation(">=");
      node.set_type("bool");
    } else if (ctx->EQ() != nullptr) {

      op_ptr = ctx->EQ();
      node.set_operation("==");
      node.set_type("bool");
    } else if (ctx->NEQ() != nullptr) {

      op_ptr = ctx->NEQ();
      node.set_operation("!=");
      node.set_type("bool");
    } else if (ctx->UARROW() != nullptr) {

      op_ptr = ctx->UARROW();
      node.set_operation("^");
    } else if (ctx->MPRSND(0) != nullptr && ctx->MPRSND(1) == nullptr) {

      op_ptr = ctx->MPRSND(0);
      node.set_operation("&");
    } else if (ctx->MPRSND(0) != nullptr && ctx->MPRSND(1) != nullptr) {

      op_ptr = ctx->MPRSND(0);
      node.set_operation("&&");
      node.set_type("bool");
    } else if (ctx->STICK(0) != nullptr && ctx->STICK(1) == nullptr) {

      op_ptr = ctx->STICK(0);
      node.set_operation("|");
    } else if (ctx->STICK(0) != nullptr && ctx->STICK(1) != nullptr) {

      op_ptr = ctx->STICK(0);
      node.set_operation("||");
      node.set_type("bool");
    }

    if (left->type() != right->type()) {
      m_error_listener->semanticError(
        op_ptr->getSymbol()->getLine(),
        op_ptr->getSymbol()->getCharPositionInLine(),
        "mismatched types: \"" + left->type() + "\" and \"" + right->type()
      );
    }

    if (node.type() == "auto") {
      node.set_type(left->type());
    }
  }
}

void VisitorInitialiser::visit(ASTAgrList& node) {
  auto* ctx = m_context.as<RustLangParser::Agr_listContext*>();

  VisitorInitialiser visitor;

  ASTExpression* unit = nullptr;

  for(auto& expr: ctx->expr_op()) {
    unit = new ASTExpression;
    visitor.set_context(expr);
    unit->accept(visitor);
    node.append_unit(unit);
  }

  /* Semantic analise */

  if (node.num_elems() == 0) {
    m_error_listener->semanticError(
      ctx->LSBR()->getSymbol()->getLine(),
      ctx->LSBR()->getSymbol()->getCharPositionInLine(),
      "Using of empty agregate list"
    );

    return;
  }

  node.set_type(node.unit(0)->type());

  auto type = node.type();

  for(auto& expr: node.units()) {
    if (expr->type() != type) {
      m_error_listener->semanticError(
        ctx->LSBR()->getSymbol()->getLine(),
        ctx->LSBR()->getSymbol()->getCharPositionInLine(),
        "Different types in agregate list"
      );

      break;
    }
  }

}

void VisitorInitialiser::visit(ASTArrAccess& node) {
  auto* ctx = m_context.as<RustLangParser::Arr_accessContext*>();

  node.set_name(ctx->ID()->getText());

  VisitorInitialiser visitor(ctx->expr_op());

  auto* expr = new ASTExpression;

  expr->accept(visitor);
  
  node.set_expression(expr);

  auto* tmp = m_table.insert(
    node.name(),
    SymbolKind::VARIABLE,
    UsageType::GET,
    "auto",
    0, 0,
    ctx->ID()->getSymbol()->getLine(),
    ctx->ID()->getSymbol()->getCharPositionInLine(),
    m_scope
  );

  if (tmp == nullptr) { return; }

  node.set_type(tmp->m_type);
  node.set_num_elems(tmp->m_num_elems);
}

}  // RustLang::parser