#include <parser/parser.hpp>
#include <codegenerator/codegenerator.hpp>

#include <cxxopts.hpp>

#include <iostream>
#include <string>

constexpr std::string_view VERSION = "0.0.1";

void printVersion() {
  std::cout << VERSION << "\n";
}

int main(int argc, const char* argv[]) {
  cxxopts::Options options("C-Rust", "Simple rust compiler");

  // clang-format off
  options.add_options()
    ("V,version", "Version")
    ("t,dump-tokens", "Output all tokens")
    ("a,dump-ast", "Output abstract syntax tree")
    ("s,dump-table", "Output symbol table")
    ("S,dump-asm", "Output assembler code")
    ("e,emit-llvm", "Output LLVM IR code")
    ("o,output", "Specify output file", cxxopts::value<std::string>()->default_value("a"))
    ("f,file", "File specification", cxxopts::value<std::string>());

  // clang-format on

  auto result = options.parse(argc, argv);

  auto version = result["version"].as<bool>();
  auto filepath = result["file"].as<std::string>();
  auto output = result["output"].as<std::string>();
  auto dump_tokens = result["dump-tokens"].as<bool>();
  auto dump_table = result["dump-table"].as<bool>();
  auto dump_ast = result["dump-ast"].as<bool>();
  auto emit_llvm = result["emit-llvm"].as<bool>();
  auto dump_asm = result["dump-asm"].as<bool>();

  if (argc == 1) {
    std::cerr << "Usage: ./app <source.rs>\n";
    return 0;
  }

  if (version) {
    printVersion();
  }

  auto parse_result = rust_lang::parser::parse(filepath);

  if (dump_tokens) {
    rust_lang::parser::dump_tokens(filepath);
  }

  if (!parse_result.m_errors.empty()) {
    rust_lang::parser::dump_errors(parse_result.m_errors);
    return 0;
  }

  if (dump_ast) {
    rust_lang::parser::dump_ast(parse_result.m_program, "ast.xml");
  }

  if (dump_table) {
    rust_lang::parser::dump_table(parse_result.m_table, "table.txt");
  }

  rust_lang::codegenerator::generate(parse_result.m_program, &parse_result.m_table, output, dump_asm, emit_llvm);

  return 0;
}
