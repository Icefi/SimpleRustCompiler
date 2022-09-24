#include <parser/parser.hpp>

#include <gtest/gtest.h>

#include <string>
#include <vector>

TEST(AstSuite, mainTest) {
  std::vector<std::string> res{
      "<program>",
      "  <function name=\"main\" return-type=\"i32\">",
      "    <args>",
      "      <variable \"argc\" type=\"constant i32\"/>",
      "    </args>",
      "    <return>",
      "      <value \"0\"/>",
      "    </return>",
      "  </function>",
      "</program>"};

  auto parse_result = rust_lang::parser::parse("../../examples/main.rs");
  rust_lang::parser::dump_ast(parse_result.m_program, "main.xml");

  std::ifstream stream("main.xml");
  std::string line;

  if (!stream.is_open()) {
    std::cerr << "File not found\n";
    exit(0);
  }

  std::size_t i = 0;
  while (std::getline(stream, line)) {
    EXPECT_EQ(line, res[i]);
    i++;
  }
}

TEST(AstSuite, funcTest) {
  std::vector<std::string> res{
      "<program>",
      "  <function name=\"bar\" return-type=\"void\">",
      "    <args>",
      "      <variable \"x\" type=\"constant i32\"/>",
      "    </args>",
      "    <call name=\"printf\">",
      "      <args>",
      "        <value \"x is %d\\n\"/>",
      "        <variable \"x\" type=\"constant i32\"/>",
      "      </args>",
      "    </call>",
      "  </function>",
      "  <function name=\"main\" return-type=\"i32\">",
      "    <call name=\"bar\">",
      "      <args>",
      "        <value \"30\"/>",
      "      </args>",
      "    </call>",
      "  </function>",
      "</program>"};

  auto parse_result = rust_lang::parser::parse("../../examples/func.rs");
  rust_lang::parser::dump_ast(parse_result.m_program, "func.xml");

  std::ifstream stream("func.xml");
  std::string line;

  if (!stream.is_open()) {
    std::cerr << "File not found\n";
    exit(0);
  }

  std::size_t i = 0;
  while (std::getline(stream, line)) {
    EXPECT_EQ(line, res[i]);
    i++;
  }
}

TEST(AstSuite, operationTest) {
  std::vector<std::string> res{
      "<program>",
      "  <function name=\"main\" return-type=\"i32\">",
      "    <return>",
      "      <operation \"+\">",
      "        <value \"3\"/>",
      "        <operation \"*\">",
      "          <value \"45\"/>",
      "          <operation \"-\">",
      "            <value \"4\"/>",
      "            <value \"2\"/>",
      "          </operation>",
      "        </operation>",
      "      </operation>",
      "    </return>",
      "  </function>",
      "</program>"};

  auto parse_result = rust_lang::parser::parse("../../examples/operation.rs");
  rust_lang::parser::dump_ast(parse_result.m_program, "operation.xml");

  std::ifstream stream("operation.xml");
  std::string line;

  if (!stream.is_open()) {
    std::cerr << "File not found\n";
    exit(0);
  }

  std::size_t i = 0;
  while (std::getline(stream, line)) {
    EXPECT_EQ(line, res[i]);
    i++;
  }
}
