#include "frontend/tokenizer.hpp"

#include <cctype>
#include <istream>
#include <locale>
#include <iostream>

bool SymbolToken::operator==(const SymbolToken& other) const {
    return name == other.name;
}

bool QuoteToken::operator==(const QuoteToken& ) const {
    return true;
}

bool DotToken::operator==(const DotToken& ) const {
    return true;
}

bool ConstantToken::operator==(const ConstantToken& other) const {
    return value == other.value;
}

Tokenizer::Tokenizer(std::istream* input)
  : _input_stream(input) {}

bool Tokenizer::IsEnd() {
        return _is_end;
    }

void Tokenizer::Next() {
  auto t = _input_stream->peek();
  if (t == std::istream::traits_type::eof()) {
    _is_end = true;
  }
}

Token Tokenizer::GetToken() {
 //auto ch = _input_stream->get();
 char ch;
 do {
    ch = _input_stream->get();
  } while (std::isspace(ch));
 switch (ch) {
  case '(':
    _input_stream->unget();
    return BracketToken::LEFT;
  case ')':
    _input_stream->unget();
    return BracketToken::RIGHT;
  case '\'':
    {
      QuoteToken token;
      _input_stream->unget();
      return token;
    }
  default:
  if (std::isalpha(ch)) { // If the character is a letter
    std::string symbol;
    while (std::isalpha(ch)) {
      symbol += ch;
      ch = _input_stream->get();
    }
    _input_stream->unget(); // Put back the non-letter character
    return SymbolToken{symbol}; // Create a SymbolToken
  } 
  if (std::isdigit(ch) || ch == '-') {
    int sign = 1;
    int value = 0;

    if (ch == '-') {
      sign = -1;
      ch = _input_stream->get();
    }

    if (!std::isdigit(ch)) {
      throw std::runtime_error("Expected a digit after '-'");
    }

    do {
      value = value * 10 + (ch - '0');
      ch = _input_stream->get();
    } while (std::isdigit(ch));

    _input_stream->unget(); // Put back the non-digit character
    return ConstantToken{value * sign};
  }  else{
      // Handle other cases if necessary or throw an error.
      std::cout << ch << "\n";
      throw std::runtime_error(std::string(&ch));
    }
  }
}
