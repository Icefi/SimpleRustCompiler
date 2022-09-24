#include "ast.hpp"

#include <stdexcept>


namespace rust_lang::parser {

antlrcpp::Any Visitor::visitExpressions(
    RustLangParser::ExpressionsContext* context) {
  if (context->assign_stmt() != nullptr) {
    return {context->assign_stmt()};
  }

  if (context->func_def() != nullptr) {
    return {context->func_def()};
  }

  if (context->mod_def() != nullptr) {
    return {context->mod_def()};
  }

  if (context->struct_def() != nullptr) {
    return {context->struct_def()};
  }

  if (context->trait_def() != nullptr) {
    return {context->trait_def()};
  }

  if (context->struct_impl() != nullptr) {
    return {context->struct_impl()};
  }

  if (context->trait_impl() != nullptr) {
    return {context->trait_impl()};
  }

  throw std::invalid_argument("Something went wrong at expressions...");
}

antlrcpp::Any Visitor::visitStatements(
    RustLangParser::StatementsContext* context) {
  if (context->assign_stmt() != nullptr) {
    return {context->assign_stmt()};
  }

  if (context->func_call() != nullptr) {
    return {context->func_call()};
  }

  if (context->var_def() != nullptr) {
    return {context->var_def()};
  }

  if (context->if_stmt() != nullptr) {
    return {context->if_stmt()};
  }

  if (context->loop_stmt() != nullptr) {
    return {context->loop_stmt()};
  }
  if (context->while_stmt() != nullptr) {
    return {context->while_stmt()};
  }
  if (context->for_stmt() != nullptr) {
    return {context->for_stmt()};
  }

  if (context->return_stmt() != nullptr) {
    return {context->return_stmt()};
  }

  if (context->break_stmt() != nullptr) {
    return {context->break_stmt()};
  }

  if (context->scope() != nullptr) {
    return {context->scope()};
  }

  throw std::invalid_argument("Something went wrong at statements...");
}

antlrcpp::Any Visitor::visitProgram(RustLangParser::ProgramContext* context) {
  return {context};
}

antlrcpp::Any Visitor::visitVar_def(RustLangParser::Var_defContext* context) {
  return {context};
}

antlrcpp::Any Visitor::visitFunc_def(RustLangParser::Func_defContext* context) {
  return {context};
}

antlrcpp::Any Visitor::visitAssign_stmt(
    RustLangParser::Assign_stmtContext* context) {
  return {context};
}

antlrcpp::Any Visitor::visitReturn_stmt(
    RustLangParser::Return_stmtContext* context) {
  return {context};
}

antlrcpp::Any Visitor::visitBreak_stmt(
    RustLangParser::Break_stmtContext* context) {
  return {context};
}

antlrcpp::Any Visitor::visitUnit(RustLangParser::UnitContext* context) {
  return {context};
}

antlrcpp::Any Visitor::visitFunc_call(
    RustLangParser::Func_callContext* context) {
  return {context};
}

antlrcpp::Any Visitor::visitScope(RustLangParser::ScopeContext* context) {
  return {context};
}

antlrcpp::Any Visitor::visitIf_stmt(RustLangParser::If_stmtContext* context) {
  return {context};
}

antlrcpp::Any Visitor::visitLoop_stmt(
    RustLangParser::Loop_stmtContext* context) {
  return {context};
}

antlrcpp::Any Visitor::visitWhile_stmt(
    RustLangParser::While_stmtContext* context) {
  return {context};
}

antlrcpp::Any Visitor::visitExpr_op(RustLangParser::Expr_opContext* context) {
  return {context};
}

antlrcpp::Any Visitor::visitFor_stmt(RustLangParser::For_stmtContext* context) {
  return {context};
}

antlrcpp::Any Visitor::visitStruct_def(
    RustLangParser::Struct_defContext* context) {
  return {context};
}

antlrcpp::Any Visitor::visitStruct_impl(
    RustLangParser::Struct_implContext* context) {
  return {context};
}

antlrcpp::Any Visitor::visitTrait_def(
    RustLangParser::Trait_defContext* context) {
  return {context};
}

antlrcpp::Any Visitor::visitTrait_impl(
    RustLangParser::Trait_implContext* context) {
  return {context};
}

antlrcpp::Any Visitor::visitVar_typedec(
    RustLangParser::Var_typedecContext* context) {
  return {context};
}

antlrcpp::Any Visitor::visitMod_def(RustLangParser::Mod_defContext* context) {
  return {context};
}

antlrcpp::Any Visitor::visitAgr_list(RustLangParser::Agr_listContext *context) {
  return {context};
}

antlrcpp::Any Visitor::visitArr_access(RustLangParser::Arr_accessContext *context) {
  return {context};
}

}
