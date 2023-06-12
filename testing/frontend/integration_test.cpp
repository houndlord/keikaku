#include "frontend/tokenizer.hpp"
#include "frontend/parser.hpp"


#include <gtest/gtest.h>
#include <sstream>


bool AstEqual(const AstNodePtr& node1, const AstNodePtr& node2) {
  if (node1 == nullptr || node2 == nullptr) {
      return node1 == node2;
  }

  // Compare NumberNode values
  auto num_node1 = std::dynamic_pointer_cast<NumberNode>(node1);
  auto num_node2 = std::dynamic_pointer_cast<NumberNode>(node2);
  if (num_node1 && num_node2) {
      return num_node1->value_ == num_node2->value_;
  }

  // Compare SymbolNode values
  auto symbol_node1 = std::dynamic_pointer_cast<SymbolNode>(node1);
  auto symbol_node2 = std::dynamic_pointer_cast<SymbolNode>(node2);
  if (symbol_node1 && symbol_node2) {
      return symbol_node1->value_ == symbol_node2->value_;
  }

  // Compare ListNode values
  auto list_node1 = std::dynamic_pointer_cast<ListNode>(node1);
  auto list_node2 = std::dynamic_pointer_cast<ListNode>(node2);
  if (list_node1 && list_node2) {
      if (list_node1->children_.size() != list_node2->children_.size()) {
          return false;
      }
      for (size_t i = 0; i < list_node1->children_.size(); i++) {
          if (!AstEqual(list_node1->children_[i], list_node2->children_[i])) {
              return false;
          }
      }
      return true;
  }

  // If none of the above checks succeeded, the nodes are of different types
  return false;
}



TEST(IntegrationTest, TestTokenizerAndParser) {
    std::string scheme_program = "(define (fact n) (if (<= n 1) 1 (* n (fact (- n 1)))))";
    std::istringstream input_stream(scheme_program);
    Tokenizer tokenizer(&input_stream);

    auto root_node = Parse(tokenizer);
    ASSERT_TRUE(root_node != nullptr);
    ASSERT_TRUE(std::dynamic_pointer_cast<ListNode>(root_node) != nullptr);

    auto program_node = std::dynamic_pointer_cast<ListNode>(root_node);
    // Assuming the first child of the root node is the definition of the 'fact' function.
    auto define_node = std::dynamic_pointer_cast<ListNode>(program_node->children_[0]);
    ASSERT_TRUE(define_node != nullptr);
    // The first child of the define node should be a SymbolNode with the value "define".
    auto define_symbol = std::dynamic_pointer_cast<SymbolNode>(define_node->children_[0]);
    ASSERT_TRUE(define_symbol != nullptr);
    ASSERT_EQ(define_symbol->value_, "define");
    // Further assertions could check the structure of the 'fact' function definition.
}

int main(int argc, char **argv) {
    ::testing::InitGoogleTest(&argc, argv);
    return RUN_ALL_TESTS();
}

TEST(IntegrationTest, TestInvalidInput) {
    std::string scheme_program = ";(define (fact n) (if (<= n 1) 1 (* n (fact (- n 1))";  // missing closing bracket
    std::istringstream input_stream(scheme_program);
    Tokenizer tokenizer(&input_stream);
    
    try {
        auto root_node = Parse(tokenizer);
        FAIL() << "Expected std::runtime_error";
    }
    catch(std::runtime_error const & err) {
        EXPECT_EQ(err.what(), std::string("Unexpected end of input"));
    }
    catch(...) {
        FAIL() << "Expected std::runtime_error";
    }
}

TEST(IntegrationTest, TestEmptyInput) {
    std::string scheme_program = "";
    std::istringstream input_stream(scheme_program);
    Tokenizer tokenizer(&input_stream);
    auto root_node = Parse(tokenizer);
    // assuming your root node children count is zero for an empty program
    EXPECT_EQ(root_node->children_.size(), 0);
}

// Test handling of single token inputs
TEST(IntegrationTest, TestSingleTokenInput) {
    // Test with single number
    {
        std::string scheme_program = "42";
        std::istringstream input_stream(scheme_program);
        Tokenizer tokenizer(&input_stream);
        auto root_node = Parse(tokenizer);
        // assuming your root node contains the parsed nodes
        ASSERT_EQ(root_node->children_.size(), 1);
        auto number_node = dynamic_cast<NumberNode*>(root_node->children_[0].get());
        ASSERT_NE(number_node, nullptr);
        EXPECT_EQ(number_node->value_, "42");
    }

    // Test with single symbol
    {
        std::string scheme_program = "hello";
        std::istringstream input_stream(scheme_program);
        Tokenizer tokenizer(&input_stream);
        auto root_node = Parse(tokenizer);
        // assuming your root node contains the parsed nodes
        ASSERT_EQ(root_node->children_.size(), 1);
        auto symbol_node = dynamic_cast<SymbolNode*>(root_node->children_[0].get());
        ASSERT_NE(symbol_node, nullptr);
        EXPECT_EQ(symbol_node->value_, "hello");
    }
}

TEST(IntegrationTest, TestComplexInput1) {
    std::string scheme_program = "(begin (define r 10) (* pi (* r r)))";
    std::istringstream input_stream(scheme_program);
    Tokenizer tokenizer(&input_stream);
    auto root_node = Parse(tokenizer);

    ASSERT_NE(root_node, nullptr);
    ASSERT_EQ(root_node->children_.size(), 3);
    auto list_node = dynamic_cast<ListNode*>(root_node->children_[1].get());
    ASSERT_NE(list_node, nullptr);
    ASSERT_EQ(list_node->children_.size(), 2);
    auto symbol_node = dynamic_cast<SymbolNode*>(list_node->children_[0].get());
    ASSERT_NE(symbol_node, nullptr);
    ASSERT_EQ(symbol_node->value_, "define");
}

TEST(IntegrationTest, TestComplexInput2) {
    std::string scheme_program = "(define (factorial n) (if (<= n 1) 1 (* n (factorial (- n 1)))))";
    std::istringstream input_stream(scheme_program);
    Tokenizer tokenizer(&input_stream);
    auto root_node = Parse(tokenizer);

    ASSERT_NE(root_node, nullptr);
    ASSERT_EQ(root_node->children_.size(), 3);
    auto list_node = dynamic_cast<ListNode*>(root_node->children_[1].get());
    ASSERT_NE(list_node, nullptr);
    ASSERT_EQ(list_node->children_.size(), 2);
    auto symbol_node = dynamic_cast<SymbolNode*>(list_node->children_[0].get());
    ASSERT_NE(symbol_node, nullptr);
    ASSERT_EQ(symbol_node->value_, "define");
}

TEST(IntegrationTest, TestComplexInput3) {
    std::string scheme_program = "(define (fib n) (cond ((= n 0) 0) ((= n 1) 1) (else (+ (fib (- n 1)) (fib (- n 2))))))";
    std::istringstream input_stream(scheme_program);
    Tokenizer tokenizer(&input_stream);
    auto root_node = Parse(tokenizer);

    ASSERT_NE(root_node, nullptr);
    ASSERT_EQ(root_node->children_.size(), 3);
    auto list_node = dynamic_cast<ListNode*>(root_node->children_[1].get());
    ASSERT_NE(list_node, nullptr);
    ASSERT_EQ(list_node->children_.size(), 2);
    auto symbol_node = dynamic_cast<SymbolNode*>(list_node->children_[0].get());
    ASSERT_NE(symbol_node, nullptr);
    ASSERT_EQ(symbol_node->value_, "define");
}

TEST(IntegrationTest, TestCorrectness1) {
    std::string scheme_program = "(define r 10)";
    std::istringstream input_stream(scheme_program);
    Tokenizer tokenizer(&input_stream);
    auto root_node = Parse(tokenizer);

    // Define the expected AST
    auto expected_root_node = std::make_shared<ListNode>();
    expected_root_node->children_.push_back(std::make_shared<SymbolNode>("define"));
    expected_root_node->children_.push_back(std::make_shared<SymbolNode>("r"));
    expected_root_node->children_.push_back(std::make_shared<NumberNode>("10"));

    // Check if the generated AST matches the expected AST
    ASSERT_TRUE(AstEqual(root_node, expected_root_node));
}

TEST(IntegrationTest, TestCorrectness2) {
    std::string scheme_program = "(+ 1 2)";
    std::istringstream input_stream(scheme_program);
    Tokenizer tokenizer(&input_stream);
    auto root_node = Parse(tokenizer);

    // Define the expected AST
    auto expected_root_node = std::make_shared<ListNode>();
    expected_root_node->children_.push_back(std::make_shared<SymbolNode>("+"));
    expected_root_node->children_.push_back(std::make_shared<NumberNode>("1"));
    expected_root_node->children_.push_back(std::make_shared<NumberNode>("2"));

    // Check if the generated AST matches the expected AST
    ASSERT_TRUE(AstEqual(root_node, expected_root_node));
}

TEST(IntegraionTest, TestCorrectnessComplex1) {
  std::string test_program = "(define (square x) (* x x)) (define (sum-of-squares x y) (+ (square x) (square y))) (sum-of-squares 3 4)";
  std::istringstream input_stream(test_program);
  Tokenizer tokenizer(&input_stream);
  auto root_node = Parse(tokenizer);
  auto expected = std::make_shared<ListNode>(std::vector<AstNodePtr>{
      std::make_shared<ListNode>(std::vector<AstNodePtr>{
          std::make_shared<SymbolNode>("define"),
          std::make_shared<ListNode>(std::vector<AstNodePtr>{
              std::make_shared<SymbolNode>("square"),
              std::make_shared<SymbolNode>("x")
          }),
          std::make_shared<ListNode>(std::vector<AstNodePtr>{
              std::make_shared<SymbolNode>("*"),
              std::make_shared<SymbolNode>("x"),
              std::make_shared<SymbolNode>("x")
          })
      }),
      std::make_shared<ListNode>(std::vector<AstNodePtr>{
          std::make_shared<SymbolNode>("define"),
          std::make_shared<ListNode>(std::vector<AstNodePtr>{
              std::make_shared<SymbolNode>("sum-of-squares"),
              std::make_shared<SymbolNode>("x"),
              std::make_shared<SymbolNode>("y")
          }),
          std::make_shared<ListNode>(std::vector<AstNodePtr>{
              std::make_shared<SymbolNode>("+"),
              std::make_shared<ListNode>(std::vector<AstNodePtr>{
                  std::make_shared<SymbolNode>("square"),
                  std::make_shared<SymbolNode>("x")
              }),
              std::make_shared<ListNode>(std::vector<AstNodePtr>{
                  std::make_shared<SymbolNode>("square"),
                  std::make_shared<SymbolNode>("y")
              })
          })
      }),
      std::make_shared<ListNode>(std::vector<AstNodePtr>{
          std::make_shared<SymbolNode>("sum-of-squares"),
          std::make_shared<NumberNode>("3"),
          std::make_shared<NumberNode>("4")
      })
  });
  ASSERT_TRUE(AstEqual(root_node, expected));
}

TEST(IntegraionTest, TestCorrectnessComplex2) {
  std::string test_program = "(define (factorial n) (if (= n 0) 1 (* n (factorial (- n 1))))) (factorial 5)";
  std::istringstream input_stream(test_program);
  Tokenizer tokenizer(&input_stream);
  auto root_node = Parse(tokenizer);
  auto expected = std::make_shared<ListNode>(std::vector<AstNodePtr>{
      std::make_shared<ListNode>(std::vector<AstNodePtr>{
          std::make_shared<SymbolNode>("define"),
          std::make_shared<ListNode>(std::vector<AstNodePtr>{
              std::make_shared<SymbolNode>("factorial"),
              std::make_shared<SymbolNode>("n")
          }),
          std::make_shared<ListNode>(std::vector<AstNodePtr>{
              std::make_shared<SymbolNode>("if"),
              std::make_shared<ListNode>(std::vector<AstNodePtr>{
                  std::make_shared<SymbolNode>("="),
                  std::make_shared<SymbolNode>("n"),
                  std::make_shared<NumberNode>("0")
              }),
              std::make_shared<NumberNode>("1"),
              std::make_shared<ListNode>(std::vector<AstNodePtr>{
                  std::make_shared<SymbolNode
                                      std::make_shared<SymbolNode>("*"),
                  std::make_shared<SymbolNode>("n"),
                  std::make_shared<ListNode>(std::vector<AstNodePtr>{
                      std::make_shared<SymbolNode>("factorial"),
                      std::make_shared<ListNode>(std::vector<AstNodePtr>{
                          std::make_shared<SymbolNode>("-"),
                          std::make_shared<SymbolNode>("n"),
                          std::make_shared<NumberNode>("1")
                      })
                  })
              })
          })
      }),
      std::make_shared<ListNode>(std::vector<AstNodePtr>{
          std::make_shared<SymbolNode>("factorial"),
          std::make_shared<NumberNode>("5")
      })
  });
  ASSERT_TRUE(AstEqual(root_node, expected));
}
