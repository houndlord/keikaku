#include <iostream>
#include <sstream>
//#pragma once

//#include "/home/houndlord/keikaku/include/frontend/tokenizer.hpp"
#include "/home/houndlord/keikaku/include/frontend/parser.hpp"
#include "/home/houndlord/keikaku/include/codegen/ir_visitor.hpp"

int main() {
  std::stringstream s{"(1 (2 (3 (4 5))))"};
  /*
  Tokenizer tokenizer(&s);
  if (tokenizer.IsEnd() == false) {
    std::cout << "alex is awesome";
  }
  auto token = tokenizer.GetToken();
  std::visit([](auto&& arg) {
    using T = std::decay_t<decltype(arg)>;
    if constexpr (std::is_same_v<T, ConstantToken>) {
        std::cout << "ConstantToken with value " << arg.value << '\n';
    }
    else if constexpr (std::is_same_v<T, BracketToken>) {
        std::cout << "BracketToken'\n'";
    }
    else if constexpr (std::is_same_v<T, SymbolToken>) {
        std::cout << "ConstantToken with value " << arg.name << '\n';
    }
    else if constexpr (std::is_same_v<T, QuoteToken>) {
        // Print something for QuoteToken
    }
    else if constexpr (std::is_same_v<T, DotToken>) {
        // Print something for DotToken
    }
}, token);
  //tokenizer.Next();
  std::cout << "t";
  auto token2 = tokenizer.GetToken();
  std::visit([](auto&& arg) {
    using T = std::decay_t<decltype(arg)>;
    if constexpr (std::is_same_v<T, ConstantToken>) {
        std::cout << "ConstantToken2 with value " << arg.value << '\n';
    }
    else if constexpr (std::is_same_v<T, BracketToken>) {
        std::cout << "BracketToken'\n'";
    }
    else if constexpr (std::is_same_v<T, SymbolToken>) {
        std::cout << "SymbolToken with value " << arg.name << '\n';
    }
    else if constexpr (std::is_same_v<T, QuoteToken>) {
        std::cout << "Q";
    }
    else if constexpr (std::is_same_v<T, DotToken>) {
        std::cout << "D";
    }
}, token2);
//tokenizer.Next();
  std::cout << "t";
  auto token3 = tokenizer.GetToken();
  std::visit([](auto&& arg) {
    using T = std::decay_t<decltype(arg)>;
    if constexpr (std::is_same_v<T, ConstantToken>) {
        std::cout << "ConstantToken2 with value " << arg.value << '\n';
    }
    else if constexpr (std::is_same_v<T, BracketToken>) {
        std::cout << "BracketToken'\n'";
    }
    else if constexpr (std::is_same_v<T, SymbolToken>) {
        std::cout << "SymbolToken with value " << arg.name << '\n';
    }
    else if constexpr (std::is_same_v<T, QuoteToken>) {
        std::cout << "Q";
    }
    else if constexpr (std::is_same_v<T, DotToken>) {
        std::cout << "D";
    }
}, token3);
  bool a;
  if (token == Token{ConstantToken{2}}) {
    a = true;
    std::cout << "true";
    std::cout << "yj";
  }
  //std::cout << a;
  std::cout << "xxyj";
  */

  Tokenizer tokenizer(&s);
  auto ast = Parse(tokenizer);
  auto visitor = std::make_shared<AstToIrVisitor>();
  auto a = visitor->GenerateIR(ast);
  for (auto& b : a->get_functions()) {
    std::cout << b;
  }
}