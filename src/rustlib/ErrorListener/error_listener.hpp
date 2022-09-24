#pragma once

#include "grammar/RustLangParser.h"

#include <vector>

struct Error {
  size_t m_line;
  size_t m_column;
  std::string m_message;
};

using Errors = std::vector<Error>; 

class StreamErrorListener : public antlr4::BaseErrorListener {
 private:
  Errors m_errors;

 public:
  void syntaxError(
      antlr4::Recognizer* /*recognizer*/,
      antlr4::Token* /*token*/,
      size_t line,
      size_t column,
      const std::string& message,
      std::exception_ptr /*e*/) override {
    m_errors.emplace_back(Error{line, column, message});
  }

  void semanticError(
    size_t line,
    size_t column,
    const std::string& message
  ) {
    m_errors.emplace_back(Error{line, column, message});
  }

  const Errors& errors() const { return m_errors; }
};


