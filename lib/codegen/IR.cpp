

#include "codegen/IR.hpp"

//std::string IRProgram::GetNextTempVar() {
//  return "temp_" + std::to_string(temp_var_counter_++);
//}

Jump::Jump(const std::shared_ptr<IRBasicBlock>& target) : target_(target) {}

const std::shared_ptr<IRBasicBlock>& Jump::GetTarget() const {
  return target_;
}

Branch::Branch(const std::shared_ptr<IRValue>& condition,
               const std::shared_ptr<IRBasicBlock>& true_target,
               const std::shared_ptr<IRBasicBlock>& false_target)
    : condition_(condition), true_target_(true_target), false_target_(false_target) {}

const std::shared_ptr<IRValue>& Branch::GetCondition() const {
  return condition_;
}

const std::shared_ptr<IRBasicBlock>& Branch::GetTrueTarget() const {
  return true_target_;
}

const std::shared_ptr<IRBasicBlock>& Branch::GetFalseTarget() const {
  return false_target_;
}