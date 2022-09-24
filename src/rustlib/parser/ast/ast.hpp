#pragma once

#include "../../symbol_table/symbol_table.hpp"

#include <grammar/RustLangParser.h>
#include <grammar/RustLangVisitor.h>

#include <map>
#include <string>
#include <vector>

namespace rust_lang::parser {

class ASTProgram;
class ASTModule;
class ASTExpression;
class ASTIf;
class ASTLoop;
class ASTWhile;
class ASTFor;
class ASTStructDefinition;
class ASTStructImplementation;
class ASTTraitDefinition;
class ASTTraitImplementation;
class ASTScope;

class ASTFunction;
class ASTCall;
class ASTValue;
class ASTArrAccess;
class ASTAgrList;
class ASTReturn;
class ASTBreak;

class ASTAssign;

class Visitor : public RustLangVisitor {
 public:
  virtual void visit(ASTProgram& node) = 0;
  virtual void visit(ASTFunction& node) = 0;
  virtual void visit(ASTValue& node) = 0;
  virtual void visit(ASTAgrList& node) = 0;
  virtual void visit(ASTArrAccess& node) = 0;
  virtual void visit(ASTModule& node) = 0;
  virtual void visit(ASTIf& node) = 0;
  virtual void visit(ASTWhile& node) = 0;
  virtual void visit(ASTFor& node) = 0;
  virtual void visit(ASTLoop& node) = 0;
  virtual void visit(ASTStructDefinition& node) = 0;
  virtual void visit(ASTStructImplementation& node) = 0;
  virtual void visit(ASTTraitDefinition& node) = 0;
  virtual void visit(ASTTraitImplementation& node) = 0;
  virtual void visit(ASTScope& node) = 0;
  virtual void visit(ASTCall& node) = 0;
  virtual void visit(ASTReturn& node) = 0;
  virtual void visit(ASTBreak& node) = 0;
  virtual void visit(ASTAssign& node) = 0;
  virtual void visit(ASTExpression& node) = 0;

  // clang-format off
 
  antlrcpp::Any visitProgram(RustLangParser::ProgramContext *context) override;
  antlrcpp::Any visitExpressions(RustLangParser::ExpressionsContext *context) override;
  antlrcpp::Any visitStatements(RustLangParser::StatementsContext *context) override;
  antlrcpp::Any visitUnit(RustLangParser::UnitContext *context) override;
  antlrcpp::Any visitFunc_call(RustLangParser::Func_callContext *context) override;
  antlrcpp::Any visitIf_stmt(RustLangParser::If_stmtContext *context) override;
  antlrcpp::Any visitWhile_stmt(RustLangParser::While_stmtContext *context) override;
  antlrcpp::Any visitExpr_op(RustLangParser::Expr_opContext *context) override;
  antlrcpp::Any visitFor_stmt(RustLangParser::For_stmtContext *context) override;
  antlrcpp::Any visitAssign_stmt(RustLangParser::Assign_stmtContext *context) override;
  // antlrcpp::Any visitAssign_op(RustLangParser::Assign_opContext *context) override;
  antlrcpp::Any visitReturn_stmt(RustLangParser::Return_stmtContext *context) override;
  antlrcpp::Any visitBreak_stmt(RustLangParser::Break_stmtContext *context) override;
  antlrcpp::Any visitScope(RustLangParser::ScopeContext *context) override;
  antlrcpp::Any visitStruct_def(RustLangParser::Struct_defContext *context) override;
  antlrcpp::Any visitStruct_impl(RustLangParser::Struct_implContext *context) override;
  antlrcpp::Any visitTrait_def(RustLangParser::Trait_defContext *context) override;
  antlrcpp::Any visitTrait_impl(RustLangParser::Trait_implContext *context) override;
  antlrcpp::Any visitLoop_stmt(RustLangParser::Loop_stmtContext *context) override;
  antlrcpp::Any visitVar_typedec(RustLangParser::Var_typedecContext *context) override;
  antlrcpp::Any visitMod_def(RustLangParser::Mod_defContext *context) override;
  antlrcpp::Any visitVar_def(RustLangParser::Var_defContext *context) override;
  antlrcpp::Any visitFunc_def(RustLangParser::Func_defContext *context) override;
  antlrcpp::Any visitAgr_list(RustLangParser::Agr_listContext *context) override;
  antlrcpp::Any visitArr_access(RustLangParser::Arr_accessContext *context) override;

  // clang-format on
};

class ASTNode {
 public:
  virtual ~ASTNode() = default;

  virtual void accept(Visitor& visitor) = 0;
};

class ASTProgram : public ASTNode {
 private:
  std::vector<ASTNode*> m_children;

 public:
  ASTProgram() = default;
  ~ASTProgram() override;

  void append_child(ASTNode* node);
  ASTNode* child(std::size_t i) const;

  std::vector<ASTNode*>& children();
  std::size_t size() const;

  void accept(Visitor& visitor) override;
};

class ASTScope : public ASTProgram {
 private:
  std::vector<ASTNode*> m_children;

 public:
  ASTScope() = default;

  void accept(Visitor& visitor) override;
};

class ASTModule : public ASTNode {
 private:
  std::string m_module_name;
  std::vector<ASTNode*> m_expressions;

 public:
  ASTModule() = default;
  ~ASTModule() override;

  std::string module_name() const;
  void set_module_name(std::string name);

  void append_expression(ASTNode* node);
  ASTNode* expression(std::size_t i) const;
  std::vector<ASTNode*>& expressions();
  std::size_t size() const;

  void accept(Visitor& visitor) override;
};

class ASTExpression : public ASTNode {
 private:
  std::string m_operation{"auto"};

  ASTExpression* m_left{nullptr};
  ASTExpression* m_right{nullptr};

  ASTNode* m_term{nullptr};

  std::string m_type{"auto"};

 public:
  ASTExpression() = default;
  ~ASTExpression() override;

  std::string operation() const;
  std::string type() const;
  bool is_leaf() const;
  ASTExpression* left() const;
  ASTExpression* right() const;
  ASTNode* term() const;


  void set_operation(const std::string& op);
  void set_type(const std::string& type);
  void set_leaf(bool leaf);
  void set_left(ASTExpression* left);
  void set_right(ASTExpression* right);
  void set_term(ASTNode* term);

  void accept(Visitor& visitor);
};

class ASTStructDefinition : public ASTNode {
 private:
  std::string m_struct_name;
  std::vector<ASTValue*> m_fields;

 public:
  ASTStructDefinition() = default;
  ~ASTStructDefinition() override;

  std::string struct_name() const;
  void set_struct_name(std::string name);

  void append_field(ASTValue* field);
  ASTValue* field(std::size_t i) const;
  std::vector<ASTValue*>& fields();
  std::size_t size() const;

  void accept(Visitor& visitor) override;
};

class ASTStructImplementation : public ASTNode {
 private:
  std::string m_struct_name;
  std::vector<ASTFunction*> m_methods;

 public:
  ASTStructImplementation() = default;

  std::string struct_name() const;

  std::vector<ASTFunction*>& methods();

  ASTFunction* method(std::size_t i) const;

  std::size_t size() const;

  void set_struct_name(std::string name);

  void append_method(ASTFunction* method);

  void accept(Visitor& visitor) override;
};

class ASTTraitDefinition : public ASTNode {
 private:
  std::string m_trait_name;
  std::vector<ASTFunction*> m_funcs;

 public:
  ASTTraitDefinition() = default;
  ~ASTTraitDefinition() override;

  std::string trait_name() const;

  std::vector<ASTFunction*>& funcs();

  ASTFunction* func(std::size_t i) const;

  std::size_t size() const;

  void set_trait_name(std::string name);

  void append_func(ASTFunction* func);

  void accept(Visitor& visitor) override;
};

class ASTTraitImplementation : public ASTTraitDefinition {
 private:
  std::string m_type_name;

 public:
  ASTTraitImplementation() = default;

  std::string type_name() const;

  void set_type_name(std::string type);

  void accept(Visitor& visitor) override;
};

class ASTFunction : public ASTNode {
 private:
  std::string m_func_name;
  std::string m_return_type{"void"};

  std::vector<ASTValue*> m_arguments;
  ASTScope* m_expression;

 public:
  ASTFunction() = default;
  ~ASTFunction() override;

  std::string func_name() const;
  std::string return_type() const;

  void set_func_name(const std::string& func_name);
  void set_return_type(const std::string& ret_type);

  ASTValue* argument(std::size_t i) const;
  std::vector<ASTValue*>& arguments();
  void append_argument(ASTValue* arg);
  std::size_t size() const;

  ASTScope* scope();
  void set_scope(ASTScope* scope);

  void accept(Visitor& visitor) override;
};

class ASTValue : public ASTNode {
 private:
  std::string m_var_name;
  std::string m_var_type{"auto"};

  bool m_is_literal{true};
  bool m_is_definition{false};
  bool m_is_mutable{false};

  std::size_t m_dimension{0};
  std::size_t m_num_elems{0};

 public:
  ASTValue() = default;

  std::string name() const;
  std::string type() const;

  void set_name(std::string name);
  void set_type(std::string type);


  bool is_literal() const;
  void set_literal(bool literal);
  
  bool is_definition() const;
  void set_definition(bool def);

  bool is_mutable() const;
  void set_mutable(bool mut);

  std::size_t dimension() const;
  void set_dimension(std::size_t dim);

  std::size_t num_elems() const;
  void set_num_elems(std::size_t num);

  void accept(Visitor& visitor) override;
};

class ASTArrAccess : public ASTNode {
 private:
  std::string m_name;
  std::string m_type{"auto"};

  std::size_t m_num_elems{1};

  ASTExpression* m_expr{nullptr};

 public:
  ASTArrAccess() = default;
  ~ASTArrAccess() override;
  
  std::string name() const;
  std::string type() const;
  std::size_t num_elems() const;
  ASTExpression* expression() const;

  void set_name(const std::string& name);
  void set_type(const std::string& type);
  void set_expression(ASTExpression* node);
  void set_num_elems(std::size_t num);

  void accept(Visitor& visitor) override;
};

class ASTCall : public ASTNode {
 private:
  std::string m_func_name;
  std::vector<ASTExpression*> m_arguments;
  std::string m_type{"void"};

 public:
  ASTCall() = default;
  ~ASTCall() override;

  std::string name() const;
  void set_name(const std::string& name);

  std::string type() const;
  void set_type(const std::string& name);

  ASTExpression* argument(std::size_t i) const;
  std::vector<ASTExpression*>& arguments();
  void append_argument(ASTExpression* arg);
  std::size_t size() const;

  void accept(Visitor& visitor) override;
};

class ASTIf : public ASTNode {
 private:
  ASTExpression* m_if_condition{nullptr};
  std::vector<ASTExpression*> m_elif_conditions;

  ASTScope* m_if_scope{nullptr};
  std::vector<ASTScope*> m_elif_scopes;
  ASTScope* m_else_scope{nullptr};

 public:
  ASTIf() = default;
  ~ASTIf() override;

  ASTExpression* if_cond();
  ASTScope* if_scope();
  ASTExpression* elif_cond(std::size_t i);
  std::vector<ASTExpression*>& elif_conds();
  ASTScope* elif_scope(std::size_t i);
  std::vector<ASTScope*>& elif_scopes();
  ASTScope* else_scope();

  void set_if_cond(ASTExpression* cond);
  void set_if_scope(ASTScope* scope);
  void append_cond(ASTExpression* expr);
  void append_scope(ASTScope* scope);
  void set_else_scope(ASTScope* scope);

  void accept(Visitor& visitor) override;
};

class ASTWhile : public ASTNode {
 private:
  ASTExpression* m_condition{nullptr};
  ASTScope* m_scope{nullptr};

 public:
  ASTWhile() = default;
  ~ASTWhile() override;

  ASTExpression* condition() const;
  void set_condition(ASTExpression* cond);

  ASTScope* scope() const;
  void set_scope(ASTScope* scope);

  void accept(Visitor& visitor) override;
};

class ASTFor : public ASTNode {
 private:
  ASTValue* m_variable{nullptr};
  ASTExpression* m_first{nullptr};
  ASTExpression* m_last{nullptr};
  ASTScope* m_scope{nullptr};

 public:
  ASTFor() = default;
  ~ASTFor();

  ASTValue* variable() const;
  ASTExpression* first() const;
  ASTExpression* last() const;
  ASTScope* scope() const;

  void set_variable(ASTValue* var);
  void set_first(ASTExpression* first);
  void set_last(ASTExpression* last);
  void set_scope(ASTScope* scope);

  void accept(Visitor& visitor);
};

class ASTLoop : public ASTNode {
 private:
  ASTScope* m_scope;

 public:
  ASTLoop() = default;
  ~ASTLoop() override;

  ASTScope* scope() const;

  void set_scope(ASTScope* scope);

  void accept(Visitor& visitor) override;
};

class ASTReturn : public ASTNode {
 protected:
  ASTNode* m_value{nullptr};

 public:
  ASTReturn() = default;
  ~ASTReturn() override;

  ASTNode* value() const;
  void set_value(ASTNode* value);

  void accept(Visitor& visitor) override;
};

class ASTBreak : public ASTReturn {
 public:
  void accept(Visitor& visitor) override;
};

class ASTAssign : public ASTNode {
 private:
  ASTNode* m_lvalue;
  ASTNode* m_rvalue;

 public:
  ASTNode* lvalue() const;
  ASTNode* rvalue() const;

  void set_lvalue(ASTNode* node);
  void set_rvalue(ASTNode* node);

  void accept(Visitor& visitor) override;
};

class ASTAgrList : public ASTNode {
 private:
  std::vector<ASTExpression*> m_units;
  std::string                 m_type{"auto"};

 public:
  ASTAgrList() = default;

  ~ASTAgrList() override;

  std::vector<ASTExpression*>& units();
  ASTExpression* unit(std::size_t i) const;
  void append_unit(ASTExpression* node);

  std::string type() const;
  void set_type(const std::string& type);

  std::size_t num_elems() const;

  void accept(Visitor& visitor) override;
};

/* Visitor Implementation */
class VisitorInitialiser : public Visitor {
 private:
  antlrcpp::Any m_context;

  static StreamErrorListener* m_error_listener;

  static SymbolTable m_table;

  static Scope m_scope;

 public:
  VisitorInitialiser() = default;
  explicit VisitorInitialiser(antlrcpp::Any context);

  void set_context(antlrcpp::Any context);

  static SymbolTable table();

  static Scope scope();
  static void increase_level(const std::string& scope);
  static void decrease_level();

  static void set_error_listener(StreamErrorListener* listener);

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
  void visit(ASTAgrList& node) override;
  void visit(ASTArrAccess& node) override;
  void visit(ASTReturn& node) override;
  void visit(ASTBreak& node) override;
  void visit(ASTAssign& node) override;
  void visit(ASTExpression& node) override;
};

class VisitorTraverse : public Visitor {
 private:
  std::ofstream& m_stream;
  static std::size_t m_intent;

 public:
  explicit VisitorTraverse(std::ofstream& file);

  static void increase_intent();
  static void decrease_intent();
  static std::size_t get_intent();

  void put_spaces() const;

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
  void visit(ASTArrAccess& node) override;
  void visit(ASTValue& node) override;
  void visit(ASTAgrList& node) override;
  void visit(ASTReturn& node) override;
  void visit(ASTBreak& node) override;
  void visit(ASTAssign& node) override;
  void visit(ASTExpression& node) override;
};

}  // RustLang::parser
