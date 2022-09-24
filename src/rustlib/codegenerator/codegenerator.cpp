#include "codegenerator.hpp"

#include <algorithm>
#include <filesystem>
#include <cstdlib>

namespace rust_lang::codegenerator {

Codegenerator::Codegenerator(std::ofstream& stream, SymbolTable* table)
    : m_stream(stream), m_table(table) {}

int Codegenerator::counter = 0;

int Codegenerator::cntr() {
  return counter;
}
void Codegenerator::reset_cntr(int c) {
  counter = c;
}

void Codegenerator::visit(ASTProgram& node) {
  m_stream << "target triple = \"x86_64-pc-linux-gnu\"\n\n";

  for (auto& child : node.children()) {
    child->accept(*this);
    m_stream << "\n";
  }

  for (auto i = 0u; i < m_strings.size(); i++) {
    m_stream << "@.str." << i << " = private unnamed_addr constant ["
             << m_strings[i].size() << " x i8] c\"" << m_strings[i]
             << "\", align 1\n";
  }

  m_stream
    << "\ndeclare i32 @printf(i8*, ...) #1\n"
    << "\ndeclare i32 @__isoc99_scanf(i8*, ...) #1\n";

  m_stream
      << "\n\nattributes #0 = { noinline nounwind optnone sspstrong uwtable "
         "\"frame-pointer\"=\"all\" \"min-legal-vector-width\"=\"0\" "
         "\"no-trapping-math\"=\"true\" "
         "\"stack-protector-buffer-size\"=\"8\" \"target-cpu\"=\"x86-64\" "
         "\"target-features\"=\"+cx8,+fxsr,+mmx,+sse,+sse2,+x87\" "
         "\"tune-cpu\"=\"generic\" }\n\n";

  m_stream << "\nattributes #1 = { \"frame-pointer\"=\"all\" "
              "\"no-trapping-math\"=\"true\" "
              "\"stack-protector-buffer-size\"=\"8\" \"target-cpu\"=\"x86-64\" "
              "\"target-features\"=\"+cx8,+fxsr,+mmx,+sse,+sse2,+x87\" "
              "\"tune-cpu\"=\"generic\" }\n\n";
}

void Codegenerator::visit(ASTModule& node) {}

void Codegenerator::visit(ASTIf& node) {

  m_stream << "\n  ; IF CONDITION\n";

  node.if_cond()->accept(*this);

  auto start_lbl = cntr() + 1;
  auto end_lbl = start_lbl + 1 + (node.else_scope() != nullptr);

  m_stream
    << "  %tmp" << cntr() << " = icmp ne i1 %tmp" << cntr() - 1 << ", 0\n";

  counter++;

  m_stream
    << "\n  ; IF BRANCHING\n"
    << "  br i1 %tmp" << cntr() - 1 << ", label %lbl" << start_lbl << ", label %lbl" << end_lbl - (node.else_scope() != nullptr) << "\n";

  m_stream
    << "\nlbl" << start_lbl << ":    ; IF SCOPE\n";

  counter++;  

  node.if_scope()->accept(*this);

  m_stream
    << "  br label %lbl" << end_lbl << "\n";

  if (node.else_scope() != nullptr) {
    m_stream << "\nlbl" << start_lbl + 1 << ":    ; ELSE SCOPE\n";

    node.else_scope()->accept(*this);

    m_stream << "  br label %lbl" << end_lbl << "\n";
  }

  m_stream << "\nlbl" << end_lbl << ":    ; END IF\n";

  counter++;

}

void Codegenerator::visit(ASTWhile& node) {
  auto cond_lbl = cntr();

  m_stream
    << "  ; WHILE\n"
    << "  br label %lbl" << cond_lbl << "\n"
    << "\nlbl" << cond_lbl << ":    ; WHILE CONDITION\n";

  counter++;

  node.condition()->accept(*this);

  m_stream
    << "  %tmp" << cntr() << " = icmp ne i1 %tmp" << cntr() - 1 << ", 0\n"
    << "  br i1 %tmp" << cntr() << ", label %lbl" << cond_lbl + 1 << ", label %lbl" << cond_lbl + 2 << "\n"
    << "\nlbl" << cond_lbl + 1 << ":    ; WHILE BODY\n";

  counter++;

  node.scope()->accept(*this);

  counter++;

  m_stream
    << "  br label %lbl" << cond_lbl << "\n"
    << "\nlbl" << cond_lbl + 2 << ":    ; END WHILE\n";

  counter++;

}

void Codegenerator::visit(ASTFor& node) {}

void Codegenerator::visit(ASTLoop& node) {}

void Codegenerator::visit(ASTStructDefinition& node) {}

void Codegenerator::visit(ASTStructImplementation& node) {}

void Codegenerator::visit(ASTTraitDefinition& node) {}

void Codegenerator::visit(ASTTraitImplementation& node) {}

void Codegenerator::visit(ASTScope& node) {
  for (auto& statement : node.children()) {
    statement->accept(*this);
  }
}

void Codegenerator::visit(ASTFunction& node) {
  if (node.func_name() == "main") {
    node.set_return_type("i32");
  }

  m_stream
    << "define " << node.return_type() << " @" << node.func_name() << "(";

  unsigned int i;
  for (i = 0; i < node.size(); i++) {
    m_stream << node.argument(i)->type() << " %" << i;

    if (i < node.size() - 1) {
      m_stream << ", ";
    }
  }

  

  m_stream << ") #0 {\n";

  counter = node.func_name() == "main" ? i : i + 1;

  for (i = 0; i < node.size(); i++) {
    m_stream << "  %" << node.argument(i)->name() << " = alloca i32\n";
    m_stream << "  store i32 %" << i << ", i32* %" << node.argument(i)->name() << "\n";
    counter++;
  }

  if (node.return_type() != "void") {
    m_stream << "  %__ret__ = alloca " << node.return_type() << "\n";
    m_stream << "  store " << node.return_type() << " 0, " << node.return_type()
             << "* %__ret__"
             << "\n\n";
    counter++;
  }

  node.scope()->accept(*this);

  m_stream << "\n  br label %__end__\n__end__:\n\n";
  if (node.return_type() == "void") {
    m_stream << "  ret void\n";
  } else if (node.return_type() == "void" && node.func_name() == "main") {
    m_stream << "  ret i32 0\n";
  } else {
    m_stream << "  %tmp" << cntr() << " = load i32, i32* %__ret__\n";
    m_stream << "  ret i32 %tmp" << cntr() << "\n";
  }

  m_stream << "}\n\n";
}

void Codegenerator::visit(ASTCall& node) {
  std::vector<int> indices;

  m_stream << "  ; FUNCTION CALL\n";

  for(auto i = 0u; i < node.size(); i++) {
    node.argument(i)->accept(*this);
    indices.push_back(counter - 1);
  }

  if (node.name() == "__isoc99_scanf") {

    m_stream
      << "  ; SCANF\n"
      << "  %tmp" << cntr() << " = call i32 (i8*, ...) @__isoc99_scanf(";
    
    m_stream << convert_to_llvm_type(node.argument(0)->type()) << " %tmp" << indices[0] << ", ";

    for (auto i = 1u; i < node.size() - 1; i++) {
      m_stream << convert_to_llvm_type(node.argument(i)->type()) << "* %" << ((ASTValue*)node.argument(i)->term())->name() << ", ";
    }

    m_stream << convert_to_llvm_type(node.argument(node.size() - 1)->type()) << "* %" << ((ASTValue*)node.argument(node.size() - 1)->term())->name();

    m_stream << ")\n\n";

    counter++;
    return;
  }

  if (node.type() != "void") {
    m_stream << "  %tmp" << cntr() << " = call ";
    counter++;
  } else {
    m_stream << "  call ";
  }
 
  m_stream << node.type() << " @" << node.name() << "(";

  if (node.size()) {
    for (auto i = 0u; i < node.size() - 1; i++) {
      m_stream << convert_to_llvm_type(node.argument(i)->type()) << " %tmp" << indices[i] << ", ";
    }

    m_stream << convert_to_llvm_type(node.argument(node.size() - 1)->type()) << " %tmp" << indices.back();
  }
  
  m_stream << ")\n\n";
}

void Codegenerator::visit(ASTValue& node) {
  // string literal
  if (node.is_literal() && node.type() == "str") {
    auto ind = append_string(node.name());
    auto str_size = m_strings[ind].size();

    m_stream
      << "  ; STRING VALUE\n"
      << "  %tmp" << cntr() << " = alloca i8*, align 8\n"
      << "  store i8* getelementptr inbounds ([" << str_size
      << " x i8], [" << str_size << " x i8]* @.str." << ind
      << ", i64 0, i64 0), i8** %tmp" << cntr() << ", align 8\n"
      << "  %tmp" << cntr() + 1 << " = load i8*, "
      << "i8** %tmp" << cntr() << "\n\n";

    counter += 2;
    return;
  }

  // variable definition
  if (!node.is_literal() && node.is_definition() && node.dimension() == 1) {

    m_stream
      << "  ; VARIABLE DEF\n"
      << "  %" << node.name() << " = alloca " << convert_to_llvm_type(node.type()) << "\n\n";

    return;
  }
  
  // get variable value
  if (!node.is_literal() && !node.is_definition() && node.dimension() == 1) {

    m_stream
      << "  ; VARIABLE GET\n"
      << "  %tmp" << cntr() << " = load " << convert_to_llvm_type(node.type()) << ", " << convert_to_llvm_type(node.type())
      << "* %" << node.name() << "\n\n";

    counter++;
    return;
  }

  // array definition
  if (!node.is_literal() && !node.is_definition() && node.dimension() == 2) {

    m_stream
      << "  ; ARRAY DEFINITION\n"
      << "  %" << node.name() << " = alloca [" << node.num_elems() << " x " << convert_to_llvm_type(node.type()) << "]\n\n";

    return;
  }

  if (node.is_literal()) {
    m_stream
      << "  ; VALUE\n"
      << "  %tmp" << cntr() << " = alloca " << convert_to_llvm_type(node.type()) << "\n"
      << "  store " << convert_to_llvm_type(node.type()) << " " << node.name() << ", "
      << convert_to_llvm_type(node.type()) << "* %tmp" << cntr() << "\n"
      << "  %tmp" << cntr() + 1 << " = load " << convert_to_llvm_type(node.type()) << ", "
      << convert_to_llvm_type(node.type()) << "* %tmp" << cntr() << "\n\n";

    counter += 2;
  }
}

void Codegenerator::visit(ASTReturn& node) {
  if (node.value() != nullptr) {
    node.value()->accept(*this);

    m_stream << "  ; return\n";
    m_stream << "  store i32 %tmp" << cntr() - 1 << ", i32* %__ret__\n";
  }

  m_stream << "  br label %__end__\n";
}

void Codegenerator::visit(ASTBreak& node) {}

void Codegenerator::visit(ASTAssign& node) {
  node.lvalue()->accept(*this);

  int rvalue_reg = 0;

  ASTValue* lvalue_v = dynamic_cast<ASTValue*>(node.lvalue());
  ASTArrAccess* lvalue_a = dynamic_cast<ASTArrAccess*>(node.lvalue());

  ASTAgrList* rvalue_as_agr = dynamic_cast<ASTAgrList*>(node.rvalue());

  std::vector<int> counters;
  if (rvalue_as_agr != nullptr) {
    for(auto& child: rvalue_as_agr->units()) {
      child->accept(*this);
      counters.push_back(counter - 1);
    }
  } else {
    node.rvalue()->accept(*this);
    rvalue_reg = cntr() - 1;
  }

  if (lvalue_v != nullptr && lvalue_v->dimension() == 1) {
    m_stream
      << "  ; VAR ASSIGNMENT\n"
      << "  store i32 %tmp"
      << rvalue_reg << ", i32* %"
      << lvalue_v->name() << "\n\n";
  } else if (lvalue_v != nullptr && lvalue_v->dimension() == 2) {

    if (lvalue_v->is_definition()) {
      m_stream
        << "  ; ARRAY ALLOCATION\n"
        << "  %" << lvalue_v->name() << " = alloca [" << lvalue_v->num_elems() << " x " << convert_to_llvm_type(lvalue_v->type()) << "]\n\n";
    }

    m_stream << "  ; ARRAY ASSIGNMENT\n";

    for(auto i = 0u; i < counters.size(); i++) {
      m_stream
        << "  %tmp" << counter << " = getelementptr inbounds ["
        << lvalue_v->num_elems() << " x " << convert_to_llvm_type(lvalue_v->type()) << "], ["
        << lvalue_v->num_elems() << " x " << convert_to_llvm_type(lvalue_v->type()) << "]* %"
        << lvalue_v->name() << ", i64 0, i64 " << i << "\n";

      m_stream
        << "  store "
        << convert_to_llvm_type(lvalue_v->type()) << " %tmp" << counters[i] << ", " 
        << convert_to_llvm_type(lvalue_v->type()) << "* %tmp" << counter << "\n";

      counter++;
    }
  } else if (lvalue_a != nullptr) {
    lvalue_a->expression()->accept(*this);
    int index_reg = counter - 1;

    if (convert_to_llvm_type(lvalue_a->expression()->type()) != "i64") {
      m_stream
        << "  %tmp" << counter << " = sext " << convert_to_llvm_type(lvalue_a->expression()->type())
        << " %tmp" << index_reg << " to i64\n";

      index_reg = counter;
      counter++;
    }

    m_stream
        << "  ; ARRAY ELEMENT ASSIGNMENT\n"
        << "  %tmp" << counter << " = getelementptr inbounds ["
        << lvalue_a->num_elems() << " x " << convert_to_llvm_type(lvalue_a->type()) << "], ["
        << lvalue_a->num_elems() << " x " << convert_to_llvm_type(lvalue_a->type()) << "]* %"
        << lvalue_a->name() << ", i64 0, i64 %tmp" << index_reg << "\n"
        << "  store " << convert_to_llvm_type(lvalue_a->type()) << " %tmp" << rvalue_reg << ", "
        << convert_to_llvm_type(lvalue_a->type()) << "* %tmp" << counter << "\n";
  }

  m_stream << "\n";
  counter++;
}

void Codegenerator::visit(ASTExpression& node) {
  if (node.left() != nullptr && node.right() != nullptr) {
    std::string op = convert_to_llvm_operation(node.operation());

    node.left()->accept(*this);
    auto lvalue = cntr() - 1;

    node.right()->accept(*this);
    auto rvalue = cntr() - 1;

    m_stream
      << "  %tmp" << cntr() << " = " << op << " "
      << convert_to_llvm_type(node.left()->type())
      << " %tmp" << lvalue << ", %tmp" << rvalue << "\n\n";

    counter++;

  } else {
    node.term()->accept(*this);
  }
}

void Codegenerator::visit(ASTAgrList& node) {}

void Codegenerator::visit(ASTArrAccess& node) {
  node.expression()->accept(*this);
  int index = counter - 1;

  if (convert_to_llvm_type(node.expression()->type()) != "i64") {
    m_stream
      << "  %tmp" << counter << " = sext " << convert_to_llvm_type(node.expression()->type())
      << " %tmp" << index << " to i64\n";

    index = counter;
    counter++;
  }

  m_stream
    << "  %tmp" << counter << " = getelementptr inbounds ["
    << node.num_elems() << " x " << convert_to_llvm_type(node.type()) << "], ["
    << node.num_elems() << " x " << convert_to_llvm_type(node.type()) << "]* %"
    << node.name() << ", i64 0, i64 %tmp" << index << "\n";

  m_stream
    << "  %tmp" << counter + 1 << " = load "
    << convert_to_llvm_type(node.type()) << ", "
    << convert_to_llvm_type(node.type()) << "* %tmp" << counter << "\n\n";

  counter += 2;
}

LLVMString::LLVMString(const std::string& str) : m_size(0), m_string(str) {

  if (str.size() == 0) {
    m_string = "\\00";
    m_size = 1;
  }

  for (auto i = 1u; i < m_string.size(); i++) {
    if (m_string[i] == 'n' && m_string[i - 1] == '\\') {
      m_string.erase(i, 1);
      m_string.insert(i, "0A");
      i++;
      continue;
    }

    m_size++;
  }

  m_string += "\\00";

  m_size += 2;
}

std::vector<LLVMString> Codegenerator::m_strings = std::vector<LLVMString>();

std::size_t Codegenerator::append_string(const std::string& str) {
  LLVMString llvm_str(str);

  auto it = std::find_if(m_strings.begin(), m_strings.end(),
    [&llvm_str](const LLVMString& lstr){
      return llvm_str == lstr;
    });

  if (it == m_strings.end()) {
    m_strings.push_back(std::move(llvm_str));
    return m_strings.size() - 1;
  }

  return it - m_strings.begin();
}

std::string LLVMString::string() const {
  return m_string;
}
std::size_t LLVMString::size() const {
  return m_size;
}

void generate(ASTProgram* node, SymbolTable* table, const std::string& filepath, bool dump_asm, bool emit_llvm) {
  std::ofstream stream(filepath + ".ll");

  Codegenerator generator(stream, table);

  node->accept(generator);

  stream.close();

  std::system(("llc " + filepath + ".ll && clang -o " + filepath + " " + filepath + ".ll").c_str());

  if (!emit_llvm) {
    try {
      if (!std::filesystem::remove(filepath + ".ll")) {
        return;
      }
    } catch(const std::filesystem::filesystem_error& err) {
      std::cout << "filesystem error: " << err.what() << '\n';
      return;
    }
  }

  if (!dump_asm) {
    try {
      if (!std::filesystem::remove(filepath + ".s")) {
        return;
      }
    } catch(const std::filesystem::filesystem_error& err) {
      std::cout << "filesystem error: " << err.what() << '\n';
      return;
    }
  }
}

}  // namespace rust_lang::codegenerator