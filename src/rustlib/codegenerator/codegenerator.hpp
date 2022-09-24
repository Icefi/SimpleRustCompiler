#pragma once

#include "../parser/ast/ast.hpp"

#include <fstream>

namespace rust_lang::codegenerator {

using namespace rust_lang::parser;

class LLVMString {
 private:
  std::string m_string;
  std::size_t m_size;

 public:
  LLVMString(const std::string& str);
  
  std::string string() const;
  std::size_t size() const;

  friend bool operator==(const LLVMString& lhs, const LLVMString& rhs) {
    return lhs.m_string == rhs.m_string && lhs.m_size == rhs.m_size;
  }

  friend bool operator!=(const LLVMString& lhs, const LLVMString& rhs) {
    return !(lhs.m_string == rhs.m_string);
  }

  friend bool operator<(const LLVMString& lhs, const LLVMString& rhs) {
    return lhs.m_string < rhs.m_string && lhs.m_size < rhs.m_size;
  }

  friend bool operator>(const LLVMString& lhs, const LLVMString& rhs) {
    return lhs.m_string > rhs.m_string && lhs.m_size > rhs.m_size;
  }

  friend std::ostream& operator<<(std::ostream& os, const LLVMString& str) {
    os << str.m_string;

    return os;
  }
};

class Codegenerator : public Visitor {
 private:
  SymbolTable* m_table;
  std::ofstream& m_stream;

  static int counter;
  static std::vector<LLVMString> m_strings;

 public:
  explicit Codegenerator(std::ofstream& stream, SymbolTable* table);


  static int  cntr();
  static void reset_cntr(int c = 0);

  static std::size_t append_string(const std::string& str);

  void visit(ASTProgram& node) override;
  void visit(ASTModule& node) override;
  void visit(ASTIf& node) override;
  void visit(ASTWhile& node) override;
  void visit(ASTFor& node) override;
  void visit(ASTLoop& node) override;
  void visit(ASTStructDefinition& node) override;
  void visit(ASTStructImplementation& node) override;
  void visit(ASTTraitDefinition& node) override;
  void visit(ASTTraitImplementation& node) override;
  void visit(ASTScope& node) override;
  void visit(ASTFunction& node) override;
  void visit(ASTCall& node) override;
  void visit(ASTValue& node) override;
  void visit(ASTReturn& node) override;
  void visit(ASTBreak& node) override;
  void visit(ASTAssign& node) override;
  void visit(ASTExpression& node) override;
  void visit(ASTAgrList& node) override;
  void visit(ASTArrAccess& node) override;
};

void generate(ASTProgram* node, SymbolTable* table, const std::string& filepath, bool dump_asm, bool emit_llvm);

}  // namespace rust_lang::codegenerator