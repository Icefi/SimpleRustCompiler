#pragma once

#include "../ErrorListener/error_listener.hpp"

#include <ostream>
#include <string>
#include <vector>
#include <map>

enum class SymbolKind {
  MODULE,
  STRUCT,
  TRAIT,
  FUNCTION,
  VARIABLE,
  NONE
};

enum class UsageType {
  DEFINITION,
  IMPLEMENTATION,
  INITIALISATION,
  ASSIGNMENT,
  CALL,
  GET,
  NONE
};

std::string kind_to_string(SymbolKind kind);

std::string usage_to_string(UsageType type);

bool is_type_trivial(const std::string& type);

std::string convert_to_llvm_type(const std::string& type);

std::string convert_to_llvm_operation(const std::string& operation);

using Scope = std::vector<std::string>;

struct SymbolNode {
  SymbolKind m_kind{SymbolKind::NONE};
  UsageType  m_usage{UsageType::NONE};

  std::string m_type{""};

  std::size_t m_dimension{1};
  std::size_t m_num_elems{1};

  std::size_t m_line{0};
  std::size_t m_col{0};

  Scope m_scope;

  SymbolNode() = default;
  SymbolNode(
    SymbolKind kind,
    UsageType usage,
    const std::string& type,
    std::size_t dimension,
    std::size_t num_elems,
    std::size_t line,
    std::size_t col,
    const Scope& scope
  );

  friend std::ostream& operator<<(std::ostream& os, const SymbolNode& node) {
    os
      << "kind: " << kind_to_string(node.m_kind) << ", "
      << "usage: " << usage_to_string(node.m_usage) << ", "
      << "type: " << node.m_type << ", "
      << "position: " << node.m_line << ":" << node.m_col << ", "
      << "dimension: " << node.m_dimension << ", "
      << "number of elements: " << node.m_num_elems << ", scope: ";

    for(auto& sc: node.m_scope) {
      os << "/" << sc;
    }
    
    return os;
  }
};

class SymbolTable {
 private:
  std::map<std::string, std::vector<SymbolNode>> m_identifiers;

  StreamErrorListener *m_error_listener{nullptr};
  
 public:
  SymbolTable() = default;

  void set_error_listener(StreamErrorListener *listener);

  std::vector<SymbolNode>* lookup(const std::string& id);

  SymbolNode* insert(
    const std::string& id,
    SymbolKind kind,
    UsageType usage,
    std::string type,
    std::size_t dimension,
    std::size_t num_elems,
    std::size_t line,
    std::size_t col,
    const Scope& scope);

  friend std::ostream& operator<<(std::ostream& os, const SymbolTable& table) {
    for(const auto& [first, second]: table.m_identifiers) {
      os << first << ":\n";

      for(auto& symbol: second) {
        os << "  " << symbol << "\n";
      }

      os << "\n";
    }

    return os;
  }
};