#include <gtest/gtest.h>
#include <sstream>

#include "tokenizer.cpp"

std::stringstream s{"123"};
class BasicTokenizerTest : public ::testing::Test{
 protected:
  Tokenizer tokenizer(&s);
};

TEST_F(BasicTokenizerTest, Seq) { 
  EXPEXT_FALSE(tokenizer.IsEnd());
  tokenizer.Next();
  EXPEXT_FALSE(tokenizer.IsEnd());
  EXPECT_EQ(tokenizer.GetToken(), Token{ConstantToken{1}});
  tokenizer.Next();
  EXPEXT_FALSE(tokenizer.IsEnd());
  EXPECT_EQ(tokenizer.GetToken(), Token{ConstantToken{2}});
  tokenizer.Next();
  EXPEXT_FALSE(tokenizer.IsEnd());
  EXPECT_EQ(tokenizer.GetToken(), Token{ConstantToken{1}});
  tokenizer.Next();
  EXPEXT_TRUE(tokenizer.IsEnd());
}
