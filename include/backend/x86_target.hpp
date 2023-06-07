#ifndef KEI_BACKEND_X86_TARGET_H
#define KEI_BACKEND_X86_TARGET_H


#include "machine_description.hpp"


std::vector<Register> x86Registers = {
  // General purpose registers
  Register("rax", 64, RegisterType::GENERAL_PURPOSE),
  Register("rbx", 64, RegisterType::GENERAL_PURPOSE),
  Register("rcx", 64, RegisterType::GENERAL_PURPOSE),
  Register("rdx", 64, RegisterType::GENERAL_PURPOSE),
  Register("rsi", 64, RegisterType::GENERAL_PURPOSE),
  Register("rdi", 64, RegisterType::GENERAL_PURPOSE),
  Register("rbp", 64, RegisterType::GENERAL_PURPOSE),
  Register("rsp", 64, RegisterType::GENERAL_PURPOSE),
  Register("r8", 64, RegisterType::GENERAL_PURPOSE),
  Register("r9", 64, RegisterType::GENERAL_PURPOSE),
  Register("r10", 64, RegisterType::GENERAL_PURPOSE),
  Register("r11", 64, RegisterType::GENERAL_PURPOSE),
  Register("r12", 64, RegisterType::GENERAL_PURPOSE),
  Register("r13", 64, RegisterType::GENERAL_PURPOSE),
  Register("r14", 64, RegisterType::GENERAL_PURPOSE),
  Register("r15", 64, RegisterType::GENERAL_PURPOSE),

  // XMM registers for floating-point and SIMD operations
  Register("xmm0", 128, RegisterType::FLOATING_POINT),
  Register("xmm1", 128, RegisterType::FLOATING_POINT),
  // ... include other xmm registers ...

  // Segment registers
  Register("cs", 16, RegisterType::SEGMENT),
  Register("ds", 16, RegisterType::SEGMENT),
  Register("ss", 16, RegisterType::SEGMENT),
  Register("es", 16, RegisterType::SEGMENT),
  Register("fs", 16, RegisterType::SEGMENT),
  Register("gs", 16, RegisterType::SEGMENT),

  // Instruction-pointer register
  Register("rip", 64, RegisterType::INSTRUCTION_POINTER),

  // Flags register
  Register("rflags", 64, RegisterType::FLAGS)
};

std::vector<Opcode> x86Opcodes = {
  Opcode("add", "0x01"),
  Opcode("sub", "0x29"),
  Opcode("mul", "0xF7"),
  Opcode("div", "0xF7"),
  Opcode("mov", "0x8B"),
  Opcode("cmp", "0x39"),
  Opcode("jmp", "0xEB"),
  Opcode("je", "0x74"),
  Opcode("jne", "0x75"),
  Opcode("jg", "0x7F"),
  Opcode("jl", "0x7C"),
  Opcode("jge", "0x7D"),
  Opcode("jle", "0x7E"),
  // ... include other opcodes as necessary...
};

MachineDesc x86MachineDesc(x86Registers, x86Opcodes);
#endif