

#include "gtest/gtest.h"


#include "codegen/ir_visitor.hppr.hpp"
#include "frontend/parser.hpp"
#include "codegen/IR.hpp"
#include "util/util.hpp"

class AstVisitorTest : public ::testing::Test {
 protected:
	AstToIrVisitor visitor;
};

TEST_F(AstToIrVisitorTest, VisitListNodeFunctionCall) {
	auto symbol_node = std::make_shared<SymbolNode>("x");
	auto number_node = std::make_shared<NumberNode>(42);
	auto list_node = std::make_shared<ListNode>();
	list_node->children_.push_back(std::make_shared<SymbolNode>("foo"));
	list_node->children_.push_back(symbol_node);
	list_node->children_.push_back(number_node);

	auto ir_program = visitor->GenerateIR(list_node);

	// Check if the IRProgram has a function named 'foo'
	auto foo_func = ir_program->GetFunction("foo");
	ASSERT_NE(foo_func, nullptr);

	// Assuming the IRFunction has a method to get its argument list
	auto args = foo_func->GetArguments();
	ASSERT_EQ(args.size(), 2);

	auto arg1 = std::dynamic_pointer_cast<IRSymbol>(args[0]);
	auto arg2 = std::dynamic_pointer_cast<IRConstant>(args[1]);
	ASSERT_NE(arg1, nullptr);
	ASSERT_NE(arg2, nullptr);
	ASSERT_EQ(arg1->GetName(), "x");
	ASSERT_EQ(arg2->GetValue(), 42);
}

TEST_F(AstToIrVisitorTest, VisitListNodeSpecialFormLet) {
    auto bindings_node = std::make_shared<ListNode>();
    bindings_node->children_.push_back(std::make_shared<ListNode>(
        std::make_shared<SymbolNode>("x"), std::make_shared<NumberNode>(42)));

    auto let_node = std::make_shared<ListNode>();
    let_node->children_.push_back(std::make_shared<SymbolNode>("let"));
    let_node->children_.push_back(bindings_node);
    let_node->children_.push_back(std::make_shared<SymbolNode>("x"));

    auto ir_program = visitor->GenerateIR(let_node);

    // Check if the IRProgram has a global variable named 'x'
    auto x_var = ir_program->GetGlobalVar("x");
    ASSERT_NE(x_var, nullptr);

    // Assuming the IRGlobalVar has a method to get its initial value
    auto init_val = x_var->GetInitialValue();
    ASSERT_NE(init_val, nullptr);

    auto const_init_val = std::dynamic_pointer_cast<IRConstant>(init_val);
    ASSERT_NE(const_init_val, nullptr);
    ASSERT_EQ(const_init_val->GetValue(), 42);
}