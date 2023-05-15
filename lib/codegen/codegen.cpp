

#include "codegen/codegen.hpp"


std::shared_ptr<IRProgram> AstToIrVisitor::GenerateIR(const AstNodePtr& ast) {
  ir_program_ = std::make_shared<IRProgram>();
  Visit(ast);
  return ir_program_;
}

void AstToIrVisitor::Visit(const AstNodePtr& node) {
  if (auto symbol_node = std::dynamic_pointer_cast<SymbolNode>(node)) {
    VisitSymbol(symbol_node);
  } else if (auto number_node = std::dynamic_pointer_cast<NumberNode>(node)) {
    VisitNumber(number_node);
  } else if (auto list_node = std::dynamic_pointer_cast<ListNode>(node)) {
    VisitList(list_node);
  } else {
    // Handle error: unexpected AST node type
  }
}

void AstToIrVisitor::VisitSymbol(const std::shared_ptr<SymbolNode>& node) {
  // Generate IR for SymbolNode
}

void AstToIrVisitor::VisitNumber(const std::shared_ptr<NumberNode>& node) {
  // Generate IR for NumberNode
}

void AstToIrVisitor::VisitList(const std::shared_ptr<ListNode>& node) {
  // Generate IR for ListNode
}
