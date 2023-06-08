#ifndef KEI_CODEGEN_IR_H
#define KEI_CODEGEN_IR_H


#include <memory>
#include <vector>
#include <string>
#include <variant>
#include <string>


enum class IRValueType {
  Constant,
  Variable,
  Temporary
};

class IRValue {
 public:
  // Constructor for constant values
  explicit IRValue(int constant_value);

  // Constructor for variable and temporary values
  explicit IRValue(const std::string& name, IRValueType type);

  // Getter for the value type
  IRValueType GetType() const;

  // Getter for the constant value
  int GetConstantValue() const;

  // Getter for the variable or temporary name
  const std::string& GetName() const;

 private:
  IRValueType type_;
  std::variant<int, std::string> value_;
};

class IRInstruction {
 public:
  virtual ~IRInstruction() = default;

  // A method to return the variables used in the instruction
  virtual std::vector<std::string> GetUsedVars() const = 0;
  virtual std::string Print() const = 0;
};

class Add : public IRInstruction {
 public:
  Add(const std::string& dest, const std::string& src1, const std::string& src2);
  std::vector<std::string> GetUsedVars() const override;
  std::string Print() const override;

  const std::string& get_dest() const;
  const std::string& get_src1() const;
  const std::string& get_src2() const;

 private:
  std::string dest_;
  std::string src1_;
  std::string src2_;
};

class Sub : public IRInstruction {
 public:
  Sub(const std::string& dest, const std::string& src1, const std::string& src2);
  std::vector<std::string> GetUsedVars() const override;
  std::string Print() const override;

  const std::string& get_dest() const;
  const std::string& get_src1() const;
  const std::string& get_src2() const;

 private:
  std::string dest_;
  std::string src1_;
  std::string src2_;
};

class Mul : public IRInstruction {
 public:
  Mul(const std::string& dest, const std::string& src1, const std::string& src2);
  std::vector<std::string> GetUsedVars() const override;
  std::string Print() const override;

  const std::string& get_dest() const;
  const std::string& get_src1() const;
  const std::string& get_src2() const;

 private:
  std::string dest_;
  std::string src1_;
  std::string src2_; 
};

class Div : public IRInstruction {
 public:
  Div(const std::string& dest, const std::string& src1, const std::string& src2);
  std::vector<std::string> GetUsedVars() const override;
  std::string Print() const override;

  const std::string& get_dest() const;
  const std::string& get_src1() const;
  const std::string& get_src2() const;

 private:
  std::string dest_;
  std::string src1_;
  std::string src2_;
};

// Similar classes for other instructions like Sub, Mul, Div, etc.

class Phi : public IRInstruction {
 public:
  Phi(const std::string& dest,
      const std::shared_ptr<IRValue>& true_value,
      const std::shared_ptr<IRValue>& false_value);
  
  Phi(const Phi& other);
  Phi(Phi&& other) noexcept;
  ~Phi() = default;

  const std::string& get_dest() const;
  const std::shared_ptr<IRValue>& get_true_value() const;
  const std::shared_ptr<IRValue>& get_false_value() const;
  
  std::vector<std::string> GetUsedVars() const override;
  std::string Print() const override;

 private:
  std::string dest_;
  std::shared_ptr<IRValue> true_value_;
  std::shared_ptr<IRValue> false_value_;
};


class IRBasicBlock {
 public:
  void add_instruction(const std::shared_ptr<IRInstruction>& instruction);
  const std::vector<std::shared_ptr<IRInstruction>>& get_instructions() const;

 private:
  std::vector<std::shared_ptr<IRInstruction>> instructions_;
};

class Jump : public IRInstruction {
 public:
  explicit Jump(const std::shared_ptr<IRBasicBlock>& target);

  const std::shared_ptr<IRBasicBlock>& GetTarget() const;
  std::vector<std::string> GetUsedVars() const override;
  std::string Print() const override;

 private:
  std::shared_ptr<IRBasicBlock> target_;
};

class Branch : public IRInstruction {
 public:
  Branch(const std::shared_ptr<IRValue>& condition,
         const std::shared_ptr<IRBasicBlock>& true_target,
         const std::shared_ptr<IRBasicBlock>& false_target);

  Branch(const Branch& other);
  Branch(Branch&& other) noexcept;
  ~Branch() = default;

  const std::shared_ptr<IRValue>& GetCondition() const;
  const std::shared_ptr<IRBasicBlock>& GetTrueTarget() const;
  const std::shared_ptr<IRBasicBlock>& GetFalseTarget() const;
  std::vector<std::string> GetUsedVars() const override;
  std::string Print() const override;

 private:
  std::shared_ptr<IRValue> condition_;
  std::shared_ptr<IRBasicBlock> true_target_;
  std::shared_ptr<IRBasicBlock> false_target_;
};

class IRFunctionCall : public IRInstruction {
 public:
  IRFunctionCall(const std::string& function_name, const std::vector<std::shared_ptr<IRValue>>& args)
      : function_name_(function_name), args_(args) {}

  const std::string& get_function_name() const { return function_name_; }
  const std::vector<std::shared_ptr<IRValue>>& get_args() const { return args_; }
  std::vector<std::string> GetUsedVars() const override;
  std::string Print() const override;

 private:
  std::string function_name_;
  std::vector<std::shared_ptr<IRValue>> args_;
};

class IRFunction {
 public:
  IRFunction(const std::string& name, const std::vector<std::string>& parameters);

  const std::string& get_name() const;
  const std::vector<std::string>& get_parameters() const;

  void add_basic_block(const std::shared_ptr<IRBasicBlock>& basic_block);
  const std::vector<std::shared_ptr<IRBasicBlock>>& get_basic_blocks() const;

  std::string Print() const;

private:
  std::string name_;
  std::vector<std::string> parameters_;
  std::vector<std::shared_ptr<IRBasicBlock>> basic_blocks_;
};

class IRProgram {
 public:
  void add_function(const std::shared_ptr<IRFunction>& function);
  const std::vector<std::shared_ptr<IRFunction>>& get_functions() const;

  std::string GetNextTempVar();

  std::string Print() const;

 private:
  std::vector<std::shared_ptr<IRFunction>> functions_;
  int temp_var_counter_ = 0;
};
#endif