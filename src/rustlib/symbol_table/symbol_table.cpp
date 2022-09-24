#include "symbol_table.hpp"

std::string kind_to_string(SymbolKind kind) {
  switch (kind) {
    case SymbolKind::MODULE:   return "module";
    case SymbolKind::STRUCT:   return "struct";
    case SymbolKind::TRAIT:    return "trait";
    case SymbolKind::FUNCTION: return "function";
    case SymbolKind::VARIABLE: return "variable";
    default:                   return "none";
  }
}

std::string usage_to_string(UsageType type) {
  switch (type) {
    case UsageType::DEFINITION:     return "definition";
    case UsageType::IMPLEMENTATION: return "implementation";
    case UsageType::INITIALISATION: return "initialisation";
    case UsageType::ASSIGNMENT:     return "assignment";
    case UsageType::CALL:           return "call";
    case UsageType::GET:            return "get";
    default:                        return "none";
  }
}

bool is_type_trivial(const std::string& type) {
  if (
    type == "i8" || type == "i16" || type == "i32" || type == "i64" || type == "i128" || type == "isize" ||
    type == "u8" || type == "u16" || type == "u32" || type == "u64" || type == "u128" || type == "usize" ||
    type == "f32" || type == "f64" || type == "f128" ||
    type == "void"
  ) { return true; }

  return false;
}

std::string convert_to_llvm_type(const std::string& type) {
  std::string llvm_type = type;

  if      (type == "i8")   { llvm_type = "i8"; }
  else if (type == "i16")  { llvm_type = "i16"; }
  else if (type == "i32")  { llvm_type = "i32"; }
  else if (type == "i64")  { llvm_type = "i64"; }
  else if (type == "i128") { llvm_type = "i128"; }
  else if (type == "f32")  { llvm_type = "float";  }
  else if (type == "f64")  { llvm_type = "double"; }
  else if (type == "f128") { llvm_type = "fp128";  }
  else if (type == "str")  { llvm_type = "i8*";    }
  else if (type == "bool") { llvm_type = "i1";     }

  return llvm_type;
}

std::string convert_to_llvm_operation(const std::string& operation) {
  std::string op = operation;

  if      (operation == "+")   { op = "add";  }
  else if (operation == "-")   { op = "sub";  }
  else if (operation == "*")   { op = "mul";  }
  else if (operation == "/")   { op = "sdiv"; }
  else if (operation == "%")   { op = "srem"; }
  else if (operation == "<<")  { op = "shl";  }
  else if (operation == ">>")  { op = "shr";  }
  else if (operation == "^")   { op = "xor";  }
  else if (operation == "<")   { op = "icmp slt"; }
  else if (operation == "<=")  { op = "icmp sle"; }
  else if (operation == ">")   { op = "icmp sgt"; }
  else if (operation == ">=")  { op = "icmp sge"; }
  else if (operation == "==")  { op = "icmp eq";  }
  else if (operation == "!=")  { op = "icmp ne";  }

  else if (operation == "&" || operation == "&&") { op = "and"; }
  else if (operation == "|" || operation == "||") { op = "or"; }

  return op;
}

SymbolNode::SymbolNode(
  SymbolKind kind,
  UsageType usage,
  const std::string& type,
  std::size_t dimension,
  std::size_t num_elems,
  std::size_t line,
  std::size_t col,
  const Scope& scope)
  : m_kind(kind), m_usage(usage), m_type(type),
    m_dimension(dimension), m_num_elems(num_elems), m_line(line), m_col(col),
    m_scope(scope) { }


std::vector<SymbolNode>* SymbolTable::lookup(const std::string& id) {
  auto it = m_identifiers.find(id);

  if (it != m_identifiers.end()) {
    return &it->second;
  } else {
    return nullptr;
  }
}

SymbolNode* SymbolTable::insert(
  const std::string& id,
  SymbolKind kind,
  UsageType usage,
  std::string type,
  std::size_t dimension,
  std::size_t num_elems,
  std::size_t line,
  std::size_t col,
  const Scope& scope)
{
  if (id == "print" || id == "scan") {
    
    m_identifiers[id].emplace_back(SymbolNode{kind, usage, type, dimension, num_elems, line, col, scope});
    return &m_identifiers[id].back();
  }

  auto it = m_identifiers.find(id);

  if (it != m_identifiers.end()) {

    if(type == "auto") {
      auto type_it = std::find_if(
        it->second.begin(),
        it->second.end(),
        [&kind, &scope](const SymbolNode& node) {
          return
            (node.m_kind == kind) &&
            (node.m_usage == UsageType::DEFINITION) &&
            (std::includes(scope.begin(), scope.end(), node.m_scope.begin(), node.m_scope.begin() + node.m_scope.size() - 1));
      });

      if (type_it == it->second.end()) {
        m_error_listener->semanticError(line, col, "The name \"" + id + "\" is not defined");
        return nullptr;
      }

      dimension = dimension == 0 ? type_it->m_dimension : dimension;
      num_elems = num_elems == 0 ? type_it->m_num_elems : num_elems;

      type = type_it->m_type;
    }


    if((std::any_of(it->second.begin(), it->second.end(),
        [&kind, &usage, &scope](const SymbolNode& node) {
          return
            (node.m_kind == kind) &&
            (node.m_usage == usage && usage == UsageType::DEFINITION) &&
            (std::includes(scope.begin(), scope.end(), node.m_scope.begin(), node.m_scope.begin() + node.m_scope.size() - 1));
        }))
    ) {
      m_error_listener->semanticError(line, col, "The name \"" + id + "\" is defined multiple times");
      return nullptr;
    }

    SymbolNode node(kind, usage, type, dimension, num_elems, line, col, scope);

    it->second.push_back(std::move(node));

    return &it->second[it->second.size() - 1];
  } else {
    if(usage == UsageType::GET || usage == UsageType::CALL) {
      m_error_listener->semanticError(line, col,
        "Cannot find value " + id + " in this scope");

      return nullptr; 
    }

    SymbolNode node(kind, usage, type, dimension, num_elems, line, col, scope);

    m_identifiers[id].push_back(std::move(node));
    return &m_identifiers[id].back();
  }
}

void SymbolTable::set_error_listener(StreamErrorListener* listener) {
  m_error_listener = listener;
}
