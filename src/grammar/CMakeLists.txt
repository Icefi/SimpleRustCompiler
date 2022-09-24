set(target_name rust_grammar)

add_library(${target_name} STATIC
  grammar/RustLangLexer.cpp
  grammar/RustLangLexer.h
  grammar/RustLangParser.cpp
  grammar/RustLangParser.h
  grammar/RustLangBaseVisitor.cpp
  grammar/RustLangBaseVisitor.h
  grammar/RustLangBaseListener.cpp
  grammar/RustLangBaseListener.h
  grammar/RustLangVisitor.cpp
  grammar/RustLangVisitor.h
  grammar/RustLangListener.cpp
  grammar/RustLangListener.h
)

include(CompileOptions)
set_compile_options(${target_name})

target_link_libraries(${target_name} PRIVATE antlr4_cpp_runtime)

target_include_directories(
  ${target_name}
  PUBLIC
    ${CMAKE_CURRENT_LIST_DIR}
)
