#include <gtest/gtest.h>
#include "codegen/ir_visitor.hpp"

// A test fixture class for the AstToIrVisitor tests
class AstToIrVisitorTest : public ::testing::Test {
 protected:
  // This method is called before each test. It's a good place to set up the
  // test environment.
  void SetUp() override {
    // Initialize the visitor. This assumes that AstToIrVisitor's constructor
    // doesn't take any parameters.
    visitor = std::make_shared<AstToIrVisitor>();
  }

  std::shared_ptr<AstToIrVisitor> visitor;
};

// A test for the VisitSymbol method
TEST_F(AstToIrVisitorTest, TestVisitSymbol) {
  // Assuming 'SymbolNode' takes a string in its constructor
  auto node = std::make_shared<SymbolNode>("myVariable");

  // Run the method under test
  auto a = visitor->GenerateIR(node);
}

// Continue with tests for other methods...

