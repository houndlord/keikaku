#ifndef KEI_CODEGEN_IR_VISITOR_H
#define KEI_CODEGEN_IR_VISITOR_H


#include <memory>
#include <string>


#include "frontend/parser.hpp"
#include "IR.hpp"
#include "util/util.hpp"


class AstToIrVisitor {
 public:
  AstToIrVisitor() = default;

  std::shared_ptr<IRProgram> GenerateIR(const AstNodePtr& ast);

 private:
  void GenerateConditionalJump(const std::shared_ptr<IRValue>& condition,
    const std::shared_ptr<IRBasicBlock>& true_block,
    const std::shared_ptr<IRBasicBlock>& false_block);

  std::shared_ptr<IRValue> GeneratePhiNode(
    const std::shared_ptr<IRValue>& true_value,
    const std::shared_ptr<IRValue>& false_value,
    const std::shared_ptr<IRBasicBlock>& merge_block);

  void Visit(const AstNodePtr& node);

  void VisitSymbol(const std::shared_ptr<SymbolNode>& node);
  void VisitNumber(const std::shared_ptr<NumberNode>& node);
  void VisitList(const std::shared_ptr<ListNode>& node);

  // Generate a function call IR instruction for a given function name and argument list
  void GenerateFunctionCall(const std::string& function_name, const std::vector<AstNodePtr>& args);

  // Get the corresponding IR value for a given AST node
  std::shared_ptr<IRValue> GetIRValue(const AstNodePtr& node);

  std::shared_ptr<IRProgram> ir_program_;
  std::shared_ptr<IRFunction> current_function_;
  std::shared_ptr<IRBasicBlock> current_basic_block_;
  std::shared_ptr<IRValue> last_value_;

  Environment environment_;
};
#endif