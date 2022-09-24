set(target_name rust_lang_parser)

add_library(${target_name} STATIC

  ErrorListener/error_listener.hpp

  parser/parser.cpp
  parser/parser.hpp

  symbol_table/symbol_table.cpp
  symbol_table/symbol_table.hpp

  parser/ast/ast.hpp
  parser/ast/nodes/program.cpp
  parser/ast/nodes/mod.cpp
  parser/ast/nodes/scope.cpp
  parser/ast/nodes/value.cpp
  parser/ast/nodes/agr_list.cpp
  parser/ast/nodes/arr_access.cpp
  parser/ast/nodes/function.cpp
  parser/ast/nodes/struct_def.cpp
  parser/ast/nodes/struct_impl.cpp
  parser/ast/nodes/trait_def.cpp
  parser/ast/nodes/trait_impl.cpp
  parser/ast/nodes/assign.cpp
  parser/ast/nodes/call.cpp
  parser/ast/nodes/return.cpp
  parser/ast/nodes/break.cpp
  parser/ast/nodes/if.cpp
  parser/ast/nodes/while.cpp
  parser/ast/nodes/for.cpp
  parser/ast/nodes/loop.cpp
  parser/ast/nodes/expr.cpp

  parser/ast/visitor.cpp
  parser/ast/visitor_initialiser.cpp
  parser/ast/visitor_traverse.cpp

  codegenerator/codegenerator.cpp
  codegenerator/codegenerator.hpp
)

include(CompileOptions)
set_compile_options(${target_name})

target_link_libraries(${target_name}
  antlr4_cpp_runtime
  rust_grammar
)

target_include_directories(
  ${target_name}
  PUBLIC
    ${CMAKE_CURRENT_LIST_DIR}
)
