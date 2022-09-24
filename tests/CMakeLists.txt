set(target_name rust_lang_test)

add_executable(
  ${target_name}
  rustlib/lexerTest.cpp
  rustlib/astTest.cpp
)

include(CompileOptions)
set_compile_options(${target_name})

target_link_libraries(
  ${target_name}
  PRIVATE
    antlr4_cpp_runtime
    rust_lang_parser
    rust_grammar
    gtest_main
)

include(GoogleTest)
gtest_discover_tests(${target_name})
