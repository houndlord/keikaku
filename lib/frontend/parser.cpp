#include "frontend/parser.hpp"

#include <cstddef>
#include <memory>
#include <string>
#include <utility>

AstNodePtr Read(Tokenizer& tokenizer, Token token) {
  //tokenizer.Next();
  if (std::holds_alternative<ConstantToken>(token)) {
    return std::make_shared<NumberNode>(
        std::to_string(std::get<ConstantToken>(token).value));
  } else if (std::holds_alternative<SymbolToken>(token)) {
    return std::make_shared<SymbolNode>(std::get<SymbolToken>(token).name);
  } else if (token == Token{BracketToken::LEFT}) {
    return ReadList(tokenizer).first;
  } else if (token == Token{BracketToken::RIGHT}) {
    throw std::runtime_error("Unexpected right bracket");
  }
}

std::pair<AstNodePtr, bool> ReadList(Tokenizer& tokenizer) { 
  auto node = std::make_shared<ListNode>();
  bool consume_next = true;

  while (!tokenizer.IsEnd()) {
    auto token = tokenizer.GetToken();
    if (token == Token{BracketToken::RIGHT}) {
      consume_next = false;
      break;
    }
    node->children_.push_back(Read(tokenizer, token));
    //tokenizer.Next();
  }
  return {node, consume_next};
}

AstNodePtr Parse(Tokenizer& tokenizer) {
  auto root = std::make_shared<ListNode>();
  while (!tokenizer.IsEnd()) {
    auto token = tokenizer.GetToken();
    bool consume_next = true;
    if (token == Token{BracketToken::LEFT}) {
      auto [node, should_consume_next] = ReadList(tokenizer);
      root->children_.push_back(node);
      consume_next = should_consume_next;
    } else if (std::holds_alternative<ConstantToken>(token) || std::holds_alternative<SymbolToken>(token)) {
      root->children_.push_back(Read(tokenizer, token));
    } else {
      throw std::runtime_error("Unexpected token");
    }
    if (consume_next) {
      //tokenizer.Next();
    }
  }
  return root;
}
