#include "backend/machine_description.hpp"


int Register::GetSize() const {
  return size_;
}

RegisterType Register::GetType() const {
  return type_;
}

const std::string& Register::GetName() const {
  return name_;
}

const std::string& Opcode::GetName() const {
  return name_;
}

const std::string& Opcode::GetBinaryRepresentation() const {
  return binary_representation_;
}

const std::vector<Register>& MachineDesc::GetRegisters() const {
  return registers_;
}

const std::vector<Opcode>& MachineDesc::GetOpcodes() const {
  return opcodes_;
}