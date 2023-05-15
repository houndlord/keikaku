#include "frontend/parser.hpp"
#include <cstddef>
#include <memory>


AstNodePtr Read(Tokenizer& tokenizer) {
  auto token = tokenizer.GetToken();
  //tokenizer.Next();
  if (std::holds_alternative<ConstantToken>(token)) {
    return std::make_shared<NumberNode>(std::to_string(std::get<ConstantToken>(token).value));
  } else if (std::holds_alternative<SymbolToken>(token)) {
    return std::make_shared<SymbolNode>(std::get<SymbolToken>(token).name);
  }
}

AstNodePtr ReadList(Tokenizer& tokenizer) {
  auto node = std::make_shared<ListNode>();
  while (true) {
    if (tokenizer.GetToken() == Token{BracketToken::RIGHT}) {
      break;
    }
    tokenizer.Next();
    node->children_.push_back(Read(tokenizer)); 
  }
  return node;
}

AstNodePtr Parse(Tokenizer& tokenizer){
  auto root = std::make_shared<ListNode>();
  while (tokenizer.IsEnd() != true) {
    auto token = tokenizer.GetToken();
    if (token == Token{BracketToken::LEFT}) {
      root->children_.push_back(ReadList(tokenizer));
    } else {
      //tokenizer.Next();
      root->children_.push_back(Read(tokenizer));
      tokenizer.Next();
    }
  }
  return root;
}