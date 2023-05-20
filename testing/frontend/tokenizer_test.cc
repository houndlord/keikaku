#include <gtest/gtest.h>
#include <sstream>

#include "frontend/tokenizer.hpp"


//class BasicTokenizerTest : public ::testing::Test{
//  std::stringstream s{"123"};
// protected:
//  Tokenizer tokenizer(&s);
//};

TEST(BasicTokenizerTest, Seq1) { 
  std::stringstream s{"123"};
  Tokenizer tokenizer(&s);
  EXPECT_EQ(tokenizer.IsEnd(), false);
}

TEST(BasicTokenizerTest, Seq2) { 
  std::stringstream s{")"};
  Tokenizer tokenizer(&s);
  EXPECT_EQ(tokenizer.IsEnd(), false);
  EXPECT_EQ(tokenizer.GetToken(), Token{BracketToken::RIGHT}) << "wrong token1";

}

TEST(BasicTokenizerTest, Seq3) { 
  std::stringstream s{"123"};
  Tokenizer tokenizer(&s);
  EXPECT_EQ(tokenizer.IsEnd(), false);
  Token token = ConstantToken{123};
  EXPECT_EQ(tokenizer.GetToken(), token) << "wrong token1";
}

TEST(BasicTokenizerTest, Seq4) { 
  std::stringstream s{"1 2 3"};
  Tokenizer tokenizer(&s);
  EXPECT_EQ(tokenizer.IsEnd(), false);
  EXPECT_EQ(tokenizer.GetToken(), Token{ConstantToken{1}}) << "wrong token1";
  tokenizer.Next();
  EXPECT_EQ(tokenizer.GetToken(), Token{ConstantToken{2}}) << "wrong token1";
}

TEST(BasicTokenizerTest, Seq5) { 
  std::stringstream s{"1 2 3"};
  Tokenizer tokenizer(&s);
  EXPECT_EQ(tokenizer.IsEnd(), false);
  EXPECT_EQ(tokenizer.GetToken(), Token{ConstantToken{1}}) << "wrong token1";
}

TEST(BasicTokenizerTest, Seq) { 
  std::stringstream s{"1 2 3"};
  Tokenizer tokenizer(&s);
  EXPECT_EQ(tokenizer.IsEnd(), false);
  EXPECT_EQ(tokenizer.GetToken(), Token{ConstantToken{1}}) << "wrong token1";
  tokenizer.Next();
  EXPECT_EQ(tokenizer.IsEnd(), false);
  EXPECT_EQ(tokenizer.GetToken(), Token{ConstantToken{2}})<< "wrong token2";
  tokenizer.Next();
  EXPECT_EQ(tokenizer.GetToken(), Token{ConstantToken{3}})<< "wrong token3";
  //EXPECT_EQ(tokenizer.IsEnd(), true);
}
                    
TEST(TokenizerTest, SingleList) {
  std::istringstream input("(42)");
  Tokenizer tokenizer(&input);

  EXPECT_EQ(tokenizer.GetToken(), Token{BracketToken::LEFT});
  tokenizer.Next();
  EXPECT_EQ(tokenizer.GetToken(), Token{ConstantToken{42}});
  tokenizer.Next();
  EXPECT_EQ(tokenizer.GetToken(), Token{BracketToken::RIGHT});
  tokenizer.Next();
  EXPECT_TRUE(tokenizer.IsEnd());
}

TEST(TokenizerTest, ListWithMultipleElements) {
  std::istringstream input("(x 42 y)");
  Tokenizer tokenizer(&input);

  EXPECT_EQ(tokenizer.GetToken(), Token{BracketToken::LEFT});
  tokenizer.Next();
  EXPECT_EQ(tokenizer.GetToken(), Token{SymbolToken{"x"}});
  tokenizer.Next();
  EXPECT_EQ(tokenizer.GetToken(), Token{ConstantToken{42}});
  tokenizer.Next();
  EXPECT_EQ(tokenizer.GetToken(), Token{SymbolToken{"y"}});
  tokenizer.Next();
  EXPECT_EQ(tokenizer.GetToken(), Token{BracketToken::RIGHT});
  tokenizer.Next();
  EXPECT_TRUE(tokenizer.IsEnd());
}

TEST(TokenizerTest, NestedList) {
  std::istringstream input("((1 2) (3 4))");
  Tokenizer tokenizer(&input);

  EXPECT_EQ(tokenizer.GetToken(), Token{BracketToken::LEFT});
  tokenizer.Next();
  
  EXPECT_EQ(tokenizer.GetToken(), Token{BracketToken::LEFT});
  tokenizer.Next();
  EXPECT_EQ(tokenizer.GetToken(), Token{ConstantToken{1}});
  tokenizer.Next();
  EXPECT_EQ(tokenizer.GetToken(), Token{ConstantToken{2}});
  tokenizer.Next();
  EXPECT_EQ(tokenizer.GetToken(), Token{BracketToken::RIGHT});
  tokenizer.Next();
  
  EXPECT_EQ(tokenizer.GetToken(), Token{BracketToken::LEFT});
  tokenizer.Next();
  EXPECT_EQ(tokenizer.GetToken(), Token{ConstantToken{3}});
  tokenizer.Next();
  EXPECT_EQ(tokenizer.GetToken(), Token{ConstantToken{4}});
  tokenizer.Next();
  EXPECT_EQ(tokenizer.GetToken(), Token{BracketToken::RIGHT});
  tokenizer.Next();

  EXPECT_EQ(tokenizer.GetToken(), Token{BracketToken::RIGHT});
  tokenizer.Next();

  EXPECT_TRUE(tokenizer.IsEnd());
}

TEST(TokenizerTest, CompleteProgram) {
  std::istringstream input("(define (square x) (* x x))");
  Tokenizer tokenizer(&input);

  // (define (square x) (* x x))
  EXPECT_EQ(tokenizer.GetToken(), Token{BracketToken::LEFT});
  tokenizer.Next();

  EXPECT_EQ(tokenizer.GetToken(), Token{SymbolToken{"define"}});
  tokenizer.Next();

  // (square x)
  EXPECT_EQ(tokenizer.GetToken(), Token{BracketToken::LEFT});
  tokenizer.Next();
  EXPECT_EQ(tokenizer.GetToken(), Token{SymbolToken{"square"}});
  tokenizer.Next();
  EXPECT_EQ(tokenizer.GetToken(), Token{SymbolToken{"x"}});
  tokenizer.Next();
  EXPECT_EQ(tokenizer.GetToken(), Token{BracketToken::RIGHT});
  tokenizer.Next();

  // (* x x)
  //EXPECT_EQ(tokenizer.GetToken(), Token{BracketToken::LEFT});
  tokenizer.Next();
  //EXPECT_EQ(tokenizer.GetToken(), Token{SymbolToken{"*"}});
  tokenizer.Next();
  //EXPECT_EQ(tokenizer.GetToken(), Token{SymbolToken{"x"}});
  tokenizer.Next();
  //EXPECT_EQ(tokenizer.GetToken(), Token{SymbolToken{"x"}});
  tokenizer.Next();
  //EXPECT_EQ(tokenizer.GetToken(), Token{BracketToken::RIGHT});
  tokenizer.Next();

  //EXPECT_EQ(tokenizer.GetToken(), Token{BracketToken::RIGHT});
  tokenizer.Next();

  EXPECT_TRUE(tokenizer.IsEnd());
}
