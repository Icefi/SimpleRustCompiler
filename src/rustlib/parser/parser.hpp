#pragma once

#include "ast/ast.hpp"

#include "../ErrorListener/error_listener.hpp"

#include <string>
#include <vector>

namespace rust_lang::parser {

struct ParseResult {
  ~ParseResult() { delete m_program; }

  static ParseResult program(ASTProgram* program, SymbolTable table) {
    ParseResult result;

    result.m_program = program;
    result.m_table = table;

    return result;
  }

  static ParseResult errors(Errors errors) {
    ParseResult result;

    result.m_errors = std::move(errors);

    return result;
  }

  ASTProgram* m_program;
  SymbolTable m_table;
  Errors      m_errors;
};

ParseResult parse(const std::string& filepath);

void dump_tokens(const std::string& filepath);
void dump_errors(Errors& errors);
void dump_ast(ASTProgram* program, const std::string& filepath);
void dump_table(SymbolTable& table, const std::string& filepath);

}  // namespace rust_lang::parser
