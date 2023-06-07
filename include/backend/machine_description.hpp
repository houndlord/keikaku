#ifndef KEI_BACKEND_MACHINE_DESCRIPTION_H
#define KEI_BACKEND_MACHINE_DESCRIPTION_H


#include <vector>
#include <string>


enum class RegisterType {
  GENERAL_PURPOSE,
  FLOATING_POINT,
  VECTOR,
  CONTROL,
  DEBUG,
  SEGMENT,
  FLAGS,
  INSTRUCTION_POINTER,
  // ... other types ...
};

class Register {
public:
  Register(const std::string& name, int size, RegisterType type);

  const std::string& GetName() const;
  int GetSize() const;
  RegisterType GetType() const;

private:
  std::string name_;
  int size_;
  RegisterType type_;
};

class Opcode {
public:
  Opcode(const std::string& name, const std::string& binary_representation);

  const std::string& GetName() const;
  const std::string& GetBinaryRepresentation() const;

private:
  std::string name_;
  std::string binary_representation_;
};

class MachineDesc {
public:
  MachineDesc(const std::vector<Register>& registers, 
              const std::vector<Opcode>& opcodes);

  const std::vector<Register>& GetRegisters() const;
  const std::vector<Opcode>& GetOpcodes() const;

private:
  std::vector<Register> registers_;
  std::vector<Opcode> opcodes_;
};
#endif