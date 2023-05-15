#include <memory>


#include "IR.hpp"
#include "frontend/parser.hpp"                    

class AstToIrVisitor {
 public:
  AstToIrVisitor() = default;

  std::shared_ptr<IRProgram> GenerateIR(const AstNodePtr& ast);

 private:
  void Visit(const AstNodePtr& node);

  void VisitSymbol(const std::shared_ptr<SymbolNode>& node);
  void VisitNumber(const std::shared_ptr<NumberNode>& node);
  void VisitList(const std::shared_ptr<ListNode>& node);

  std::shared_ptr<IRProgram> ir_program_;
  std::shared_ptr<IRFunction> current_function_;
  std::shared_ptr<IRBasicBlock> current_basic_block_;
};