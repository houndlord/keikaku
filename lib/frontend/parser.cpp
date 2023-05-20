#include "frontend/parser.hpp"
#include <cstddef>
#include <memory>
#include <string>


AstNodePtr Read(Tokenizer& tokenizer, Token token) {
  tokenizer.Next();
  if (std::holds_alternative<ConstantToken>(token)) {
    return std::make_shared<NumberNode>(std::to_string(std::get<ConstantToken>(token).value));
  } else if (std::holds_alternative<SymbolToken>(token)) {
    return std::make_shared<SymbolNode>(std::get<SymbolToken>(token).name);
  } else if (token == Token{BracketToken::LEFT}) {
    // You could throw an exception here, or handle the unexpected token type in some other way
    //throw std::runtime_error("Unexpected token type");
    return ReadList(tokenizer);
  } else if (token == Token{BracketToken::RIGHT}) {
    throw std::runtime_error("RIGHT!!");
  }
}

AstNodePtr ReadList(Tokenizer& tokenizer) {
  auto node = std::make_shared<ListNode>();
  // Consume the left bracket token
  tokenizer.Next();  

  while (true) {
    // Check the next token without consuming it
    auto token = tokenizer.GetToken();

    // If the next token is the right bracket, consume it and break the loop
    if (token == Token{BracketToken::RIGHT}) {
      tokenizer.Next();  
      break;
    }
    if (token == Token{BracketToken::LEFT}) {
      tokenizer.Next();
      node->children_.push_back(ReadList(tokenizer));
    }

    // Otherwise, consume the token and add the result of Read to the children of node
    tokenizer.Next();
    node->children_.push_back(Read(tokenizer, token)); 
  }
  return node;
}


AstNodePtr Parse(Tokenizer& tokenizer){
  auto root = std::make_shared<ListNode>();
  while (!tokenizer.IsEnd()) {
    auto token = tokenizer.GetToken();
    if (token == Token{BracketToken::LEFT}) {
      root->children_.push_back(ReadList(tokenizer));
    } else if (std::holds_alternative<ConstantToken>(token) || std::holds_alternative<SymbolToken>(token)) {
      root->children_.push_back(Read(tokenizer, token));
    } else {
      // You could throw an exception here, or handle the unexpected token type in some other way
      throw std::runtime_error("xyj");
    }
  }
  return root;
}
