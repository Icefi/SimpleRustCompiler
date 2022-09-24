#include "parser.hpp"

#include "grammar/RustLangLexer.h"
#include "grammar/RustLangParser.h"

#include <antlr4-runtime.h>

#include <exception>
#include <iostream>

namespace rust_lang::parser {



void dump_tokens(const std::string& filepath) {
  std::ifstream stream(filepath);

  if (!stream.is_open()) {
    throw std::ios_base::failure(
        std::string("error: couldn't read ") + filepath +
        std::string(": No such file or directory (os error 2)\n"));
  }

  antlr4::ANTLRInputStream input(stream);

  RustLangLexer lexer(&input);

  auto vocabulary = lexer.getVocabulary();
  while (true) {
    auto tkn = lexer.nextToken();

    if (tkn->getText() == "<EOF>") {
      break;
    }

    std::cout << "Loc=<" << tkn->getLine() << ", "
              << tkn->getCharPositionInLine() << "> "
              << vocabulary.getSymbolicName(tkn->getType()) << " \'"
              << tkn->getText() << "\'\n";
  }
}

ParseResult parse(const std::string& filepath) {
  std::ifstream stream(filepath);

  if (!stream.is_open()) {
    throw std::ios_base::failure(
        "error: couldn't read file: No such file or directory (os error 2)\n");
  }

  antlr4::ANTLRInputStream input(stream);
  RustLangLexer lexer(&input);

  antlr4::CommonTokenStream tokens(&lexer);
  RustLangParser parser(&tokens);

  /* Error listener */
  StreamErrorListener error_listener;
  parser.removeErrorListeners();
  parser.addErrorListener(&error_listener);

  antlrcpp::Any program = parser.program();

  const auto& syntax_errors = error_listener.errors();
  if (!syntax_errors.empty()) {
    return ParseResult::errors(syntax_errors);
  }

  VisitorInitialiser visitor_init(program);
  visitor_init.set_error_listener(&error_listener);

  auto* ast = new ASTProgram;

  ast->accept(visitor_init);

  auto table = visitor_init.table();

  const auto& semantic_errors = error_listener.errors();
  if (!semantic_errors.empty()) {
    return ParseResult::errors(semantic_errors);
  }

  return ParseResult::program(ast, std::move(table));
}

void dump_errors(Errors& errors) {
  for (const auto& error : errors) {
    std::cerr << "Error at: " << error.m_line << " " << error.m_column << " :"
              << error.m_message << "\n";
  }
  std::cerr << "\n";
}

void dump_ast(ASTProgram* program, const std::string& filepath) {
  std::ofstream stream(filepath);

  VisitorTraverse visitor_print(stream);
  program->accept(visitor_print);
}

void dump_table(SymbolTable& table, const std::string& filepath) {
  std::ofstream stream(filepath);

  stream << table << "\n";
}

}  // namespace rust_lang::parser
