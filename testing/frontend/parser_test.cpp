#include <gtest/gtest.h>
#include <sstream>


#include "frontend/parser.hpp"
//#include "frontend/tokenizer.hpp"


TEST(ParserTest, ReadNumber) {
  std::istringstream input("42");
  Tokenizer tokenizer(&input);
  auto token = tokenizer.GetToken();
  auto ast = Read(tokenizer, token);
  auto number_node = std::dynamic_pointer_cast<NumberNode>(ast);
  ASSERT_NE(number_node, nullptr);
  EXPECT_EQ(number_node->value_, "42");
}

TEST(ParserTest, ReadSymbol) {
  std::istringstream input("x");
  Tokenizer tokenizer(&input);
  auto token = tokenizer.GetToken();
  auto ast = Read(tokenizer, token);
  auto symbol_node = std::dynamic_pointer_cast<SymbolNode>(ast);
  ASSERT_NE(symbol_node, nullptr);
  EXPECT_EQ(symbol_node->value_, "x");
}

TEST(ParserTest, ReadEmptyInput) {
  std::istringstream input("");
  Tokenizer tokenizer(&input);
  ASSERT_THROW(tokenizer.GetToken(), std::runtime_error);  // GetToken should throw an error on empty input.
}

TEST(ParserTest, ReadInvalidInput) {
  std::istringstream input("` .");
  Tokenizer tokenizer(&input);
  ASSERT_THROW(tokenizer.GetToken(), std::runtime_error);  // GetToken should throw an error on invalid input.
}

TEST(ParserTest, HandleEndOfInput) {
  std::istringstream input("123");
  Tokenizer tokenizer(&input);
  auto token = tokenizer.GetToken();  // Move to the first token.
  auto ast = Read(tokenizer, token);
  ASSERT_NE(ast, nullptr);  // We expect a valid AST node here.
  auto number_node = std::dynamic_pointer_cast<NumberNode>(ast);
  ASSERT_NE(number_node, nullptr);  // We expect a NumberNode.
  EXPECT_EQ(number_node->value_, "123");

  ASSERT_THROW(tokenizer.GetToken(), std::runtime_error);  // GetToken should throw an error when trying to move beyond the end of input.
}

TEST(ParserTest, ReadList) {
  std::istringstream input("(x 42)");
  Tokenizer tokenizer(&input);
  tokenizer.GetToken();  // Skip the initial left bracket.
  auto ast = ReadList(tokenizer);
  auto list_node = std::dynamic_pointer_cast<ListNode>(ast.first);
  ASSERT_NE(list_node, nullptr);
  ASSERT_EQ(list_node->children_.size(), 2);

  auto symbol_node = std::dynamic_pointer_cast<SymbolNode>(list_node->children_[0]);
  ASSERT_NE(symbol_node, nullptr);
  EXPECT_EQ(symbol_node->value_, "x");

  auto number_node = std::dynamic_pointer_cast<NumberNode>(list_node->children_[1]);
  ASSERT_NE(number_node, nullptr);
  EXPECT_EQ(number_node->value_, "42");
}

TEST(ParserTest, ParseSingleNumber) {
  std::istringstream input("42");
  Tokenizer tokenizer(&input);
  auto ast = Parse(tokenizer);
  auto list_node = std::dynamic_pointer_cast<ListNode>(ast);
  ASSERT_NE(list_node, nullptr);
  ASSERT_EQ(list_node->children_.size(), 1);
  auto number_node = std::dynamic_pointer_cast<NumberNode>(list_node->children_[0]);
  ASSERT_NE(number_node, nullptr);
  EXPECT_EQ(number_node->value_, "42");
}

TEST(ParserTest, ParseSingleSymbol) {
  std::istringstream input("x");
  Tokenizer tokenizer(&input);
  auto ast = Parse(tokenizer);
  auto list_node = std::dynamic_pointer_cast<ListNode>(ast);
  ASSERT_NE(list_node, nullptr);
  ASSERT_EQ(list_node->children_.size(), 1);
  auto symbol_node = std::dynamic_pointer_cast<SymbolNode>(list_node->children_[0]);
  ASSERT_NE(symbol_node, nullptr);
  EXPECT_EQ(symbol_node->value_, "x");
} 

TEST(ParserTest, HandleEndOfInputInParse) {
  std::istringstream input("123");
  Tokenizer tokenizer(&input);
  tokenizer.Next();  // Move to the first token.
  
  auto ast = Parse(tokenizer);
  ASSERT_NE(ast, nullptr);  // We expect a valid AST node here.
  auto number_node = std::dynamic_pointer_cast<NumberNode>(ast);
  ASSERT_NE(number_node, nullptr);  // We expect a NumberNode.
  EXPECT_EQ(number_node->value_, "123");

  ast = Parse(tokenizer);  // Try to parse beyond the end of input.
  EXPECT_EQ(ast, nullptr);  // We expect parse to return null or equivalent.
}

TEST(ParserTest, ParseList) {
  std::istringstream input("(x 42)");
  Tokenizer tokenizer(&input);
  auto ast = Parse(tokenizer);
  auto list_node = std::dynamic_pointer_cast<ListNode>(ast);
  ASSERT_NE(list_node, nullptr);
  
  auto inner_list_node = std::dynamic_pointer_cast<ListNode>(list_node->children_[0]);
  ASSERT_EQ(inner_list_node->children_.size(), 2);

  auto symbol_node = std::dynamic_pointer_cast<SymbolNode>(inner_list_node->children_[0]);
  ASSERT_NE(symbol_node, nullptr);
  EXPECT_EQ(symbol_node->value_, "x");

  auto number_node = std::dynamic_pointer_cast<NumberNode>(inner_list_node->children_[1]);
  ASSERT_NE(number_node, nullptr);
  EXPECT_EQ(number_node->value_, "42");
}

TEST(ParserTest, ReadNestedList) {
  std::istringstream input("((x 42) (y 24))");
  Tokenizer tokenizer(&input);
  //tokenizer.Next();  // Skip the initial left bracket.
  auto ast = ReadList(tokenizer);
  auto list_node = std::dynamic_pointer_cast<ListNode>(ast.first);
  ASSERT_NE(list_node, nullptr);
  ASSERT_EQ(list_node->children_.size(), 2);

  // Check the first nested list.
  //auto nested_list_node1 = std::dynamic_pointer_cast<ListNode>(list_node->children_[0]);
  //ASSERT_NE(nested_list_node1, nullptr);
  //ASSERT_EQ(nested_list_node1->children_.size(), 2);
  // Check the contents of the first nested list...
  
  // Check the second nested list.
  //auto nested_list_node2 = std::dynamic_pointer_cast<ListNode>(list_node->children_[1]);
  //ASSERT_NE(nested_list_node2, nullptr);
  //ASSERT_EQ(nested_list_node2->children_.size(), 2);
  // Check the contents of the second nested list...
}

TEST(ParserTest, ParseComplexNestedStructure1) {
  std::istringstream input("((123 abc) (a def))");
  Tokenizer tokenizer(&input);
  //tokenizer.Next();  // Move to the first token.

  auto ast = Parse(tokenizer);
  ASSERT_NE(ast, nullptr);

  auto list_node = std::dynamic_pointer_cast<ListNode>(ast);
  ASSERT_NE(list_node, nullptr);
  //ASSERT_EQ(list_node->children_.size(), 2);

  auto inner_list_node = std::dynamic_pointer_cast<ListNode>(list_node->children_[0]);

  // Check the first child.
  auto first_child = std::dynamic_pointer_cast<ListNode>(inner_list_node->children_[0]);
  ASSERT_NE(first_child, nullptr);
  ASSERT_EQ(first_child->children_.size(), 2);

  auto first_child_first_subchild = std::dynamic_pointer_cast<NumberNode>(first_child->children_[0]);
  ASSERT_NE(first_child_first_subchild, nullptr);
  EXPECT_EQ(first_child_first_subchild->value_, "123");

  auto first_child_second_subchild = std::dynamic_pointer_cast<SymbolNode>(first_child->children_[1]);
  ASSERT_NE(first_child_second_subchild, nullptr);
  EXPECT_EQ(first_child_second_subchild->value_, "abc");

  // Check the second child.
  auto second_child = std::dynamic_pointer_cast<ListNode>(inner_list_node->children_[1]);
  ASSERT_NE(second_child, nullptr);
  ASSERT_EQ(second_child->children_.size(), 2);

  auto second_child_first_subchild = std::dynamic_pointer_cast<AstNode>(second_child->children_[0]);
  ASSERT_NE(second_child_first_subchild, nullptr);

  auto second_child_second_subchild = std::dynamic_pointer_cast<SymbolNode>(second_child->children_[1]);
  ASSERT_NE(second_child_second_subchild, nullptr);
  EXPECT_EQ(second_child_second_subchild->value_, "def");
}

TEST(ParserTest, ParseComplexNestedStructure2) {
  std::istringstream input("(1 (2 (3 (4 5))))");
  Tokenizer tokenizer(&input);
  tokenizer.Next();  // Move to the first token.

  auto ast = Parse(tokenizer);
  ASSERT_NE(ast, nullptr);

  auto list_node = std::dynamic_pointer_cast<ListNode>(ast);
  ASSERT_NE(list_node, nullptr);
  ASSERT_EQ(list_node->children_.size(), 1);

  auto first_child = std::dynamic_pointer_cast<NumberNode>(list_node->children_[0]);
  //ASSERT_NE(first_child, nullptr);
  EXPECT_EQ(first_child->value_, "1");

  // Check the second child.
  //auto second_child = list_node->children_[1];

}

TEST(ParserTest, ParseSimpleProgram) {
  std::istringstream input("(define square (lambda (x) (* x x)))");
  Tokenizer tokenizer(&input);
  tokenizer.Next();  // Move to the first token.

  auto ast = Parse(tokenizer);
  ASSERT_NE(ast, nullptr);

  auto list_node = std::dynamic_pointer_cast<ListNode>(ast);
  ASSERT_NE(list_node, nullptr);
  ASSERT_EQ(list_node->children_.size(), 3);

  auto first_child = std::dynamic_pointer_cast<SymbolNode>(list_node->children_[0]);
  ASSERT_NE(first_child, nullptr);
  EXPECT_EQ(first_child->value_, "define");

  auto second_child = std::dynamic_pointer_cast<SymbolNode>(list_node->children_[1]);
  ASSERT_NE(second_child, nullptr);
  EXPECT_EQ(second_child->value_, "square");

  auto third_child = std::dynamic_pointer_cast<ListNode>(list_node->children_[2]);
  ASSERT_NE(third_child, nullptr);
  ASSERT_EQ(third_child->children_.size(), 3);

  auto third_child_first_subchild = std::dynamic_pointer_cast<SymbolNode>(third_child->children_[0]);
  ASSERT_NE(third_child_first_subchild, nullptr);
  EXPECT_EQ(third_child_first_subchild->value_, "lambda");

  auto third_child_second_subchild = std::dynamic_pointer_cast<ListNode>(third_child->children_[1]);
  ASSERT_NE(third_child_second_subchild, nullptr);
  ASSERT_EQ(third_child_second_subchild->children_.size(), 1);

  auto third_child_third_subchild = std::dynamic_pointer_cast<ListNode>(third_child->children_[2]);
  ASSERT_NE(third_child_third_subchild, nullptr);
  ASSERT_EQ(third_child_third_subchild->children_.size(), 3);
}

TEST(ParserTest, ParseComplexProgram) {
  std::istringstream input("(define (factorial n) (if (= n 0) 1 (* n (factorial (- n 1)))))");
  Tokenizer tokenizer(&input);
  tokenizer.Next();  // Move to the first token.

  auto ast = Parse(tokenizer);
  ASSERT_NE(ast, nullptr);

  auto list_node = std::dynamic_pointer_cast<ListNode>(ast);
  ASSERT_NE(list_node, nullptr);
  ASSERT_EQ(list_node->children_.size(), 2);

  // Test the definition of the factorial function
  auto first_child = std::dynamic_pointer_cast<ListNode>(list_node->children_[0]);
  ASSERT_NE(first_child, nullptr);
  EXPECT_EQ(first_child->children_.size(), 3);

  auto first_child_first_subchild = std::dynamic_pointer_cast<SymbolNode>(first_child->children_[0]);
  ASSERT_NE(first_child_first_subchild, nullptr);
  EXPECT_EQ(first_child_first_subchild->value_, "define");

  auto first_child_second_subchild = std::dynamic_pointer_cast<ListNode>(first_child->children_[1]);
  ASSERT_NE(first_child_second_subchild, nullptr);
  EXPECT_EQ(first_child_second_subchild->children_.size(), 2);

  // Test the invocation of the factorial function
  auto second_child = std::dynamic_pointer_cast<ListNode>(list_node->children_[1]);
  ASSERT_NE(second_child, nullptr);
  EXPECT_EQ(second_child->children_.size(), 2);

  auto second_child_first_subchild = std::dynamic_pointer_cast<SymbolNode>(second_child->children_[0]);
  ASSERT_NE(second_child_first_subchild, nullptr);
  EXPECT_EQ(second_child_first_subchild->value_, "factorial");

  auto second_child_second_subchild = std::dynamic_pointer_cast<NumberNode>(second_child->children_[1]);
  ASSERT_NE(second_child_second_subchild, nullptr);
  EXPECT_EQ(second_child_second_subchild->value_, "5");
}