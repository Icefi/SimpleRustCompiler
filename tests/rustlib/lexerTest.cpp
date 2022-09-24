#include <gtest/gtest.h>

#include <grammar/RustLangLexer.h>

#include <string>
#include <vector>

struct TokenInfo {
  std::size_t line;
  std::size_t col;
  std::size_t type;
  std::string text;

  TokenInfo() : line(0), col(0), type(0) {}
};

std::vector<TokenInfo> tkns;

std::vector<TokenInfo> lexer_test(std::string& arg) {
  std::vector<TokenInfo> result;

  std::ifstream stream(arg);

  if (!stream.is_open()) {
    std::cerr << "File not found\n";
    exit(0);
  }

  antlr4::ANTLRInputStream input(stream);

  RustLangLexer lexer(&input);

  TokenInfo tkn_info;

  while (true) {
    auto tkn = lexer.nextToken();

    if (tkn->getText() == "<EOF>") {
      break;
    }

    tkn_info.line = tkn->getLine();
    tkn_info.col = tkn->getCharPositionInLine();
    tkn_info.type = tkn->getType();
    tkn_info.text = tkn->getText();

    result.push_back(tkn_info);
  }

  return result;
}

TEST(LexerTest, TokenNameTest) {
  std::string path("../../examples/foo.rs");

  tkns = lexer_test(path);

  EXPECT_EQ(tkns[0].text, "fn");
  EXPECT_EQ(tkns[1].text, "foo");
  EXPECT_EQ(tkns[2].text, "(");
  EXPECT_EQ(tkns[3].text, ")");
  EXPECT_EQ(tkns[4].text, "{");
  EXPECT_EQ(tkns[5].text, "let");
  EXPECT_EQ(tkns[6].text, "a");
  EXPECT_EQ(tkns[7].text, "=");
  EXPECT_EQ(tkns[8].text, "3");
  EXPECT_EQ(tkns[9].text, ";");
  EXPECT_EQ(tkns[10].text, "a");
  EXPECT_EQ(tkns[11].text, "}");
}

TEST(LexerTest, TokenLineTest) {
  EXPECT_EQ(tkns[0].line, 1);
  EXPECT_EQ(tkns[1].line, 1);
  EXPECT_EQ(tkns[2].line, 1);
  EXPECT_EQ(tkns[3].line, 1);
  EXPECT_EQ(tkns[4].line, 1);
  EXPECT_EQ(tkns[5].line, 2);
  EXPECT_EQ(tkns[6].line, 2);
  EXPECT_EQ(tkns[7].line, 2);
  EXPECT_EQ(tkns[8].line, 2);
  EXPECT_EQ(tkns[9].line, 2);
  EXPECT_EQ(tkns[10].line, 3);
  EXPECT_EQ(tkns[11].line, 4);
}

TEST(LexerTest, TokenColTest) {
  EXPECT_EQ(tkns[0].col, 0);
  EXPECT_EQ(tkns[1].col, 3);
  EXPECT_EQ(tkns[2].col, 6);
  EXPECT_EQ(tkns[3].col, 7);
  EXPECT_EQ(tkns[4].col, 9);
  EXPECT_EQ(tkns[5].col, 4);
  EXPECT_EQ(tkns[6].col, 8);
  EXPECT_EQ(tkns[7].col, 10);
  EXPECT_EQ(tkns[8].col, 12);
  EXPECT_EQ(tkns[9].col, 13);
  EXPECT_EQ(tkns[10].col, 4);
  EXPECT_EQ(tkns[11].col, 0);
}
