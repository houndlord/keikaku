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
  return _input_stream->peek() == std::char_traits<char>::eof();
}

void Tokenizer::Next() {
  _input_stream->get();
  _is_end = _input_stream->peek() == std::char_traits<char>::eof();
  _input_stream->unget();
}

Token Tokenizer::GetToken() {
  char ch;
  // Continue reading whitespace characters until a non-whitespace character is encountered
  do {
    if (_input_stream->eof()) {  // Check if end of file
      throw std::runtime_error("Unexpected end of file");
    }
    ch = _input_stream->get();
  } while (std::isspace(ch) && ch != '(' && ch != ')');  // Don't consume parentheses here

  // Process non-whitespace characters
  switch (ch) {
    case '(':
      return BracketToken::LEFT;
    case ')':
      return BracketToken::RIGHT;
    case '\'':
      return QuoteToken{};
    default:
      if (std::isalpha(ch) || ch == '+' || ch == '-' || ch == '*' || ch == '/') { 
        std::string symbol;
        do {
          symbol += ch;
          if (_input_stream->eof()) { // Check for EOF before getting next char
            break;
          }
          ch = _input_stream->get();
        } while (std::isalpha(ch) || ch == '+' || ch == '-' || ch == '*' || ch == '/');
        if (ch != std::char_traits<char>::eof() && !std::isspace(ch)) _input_stream->unget(); // Put back the non-digit character if it's not a space or EOF
        return SymbolToken{symbol};
      } 
      if (std::isdigit(ch) || ch == '-') {
        int sign = 1;
        int value = 0;

        if (ch == '-') {
          sign = -1;
          if (_input_stream->peek() == std::istream::traits_type::eof()) { // Check for EOF before getting next char
            throw std::runtime_error("Expected a digit after '-'");
          }
          ch = _input_stream->get();
        }

        if (!std::isdigit(ch)) {
          throw std::runtime_error("Expected a digit after '-'");
        }

        do {
          value = value * 10 + (ch - '0');
          if (_input_stream->eof()) { // Check for EOF before getting next char
            break;
          }
          ch = _input_stream->get();
        } while (std::isdigit(ch));
        if (ch != std::char_traits<char>::eof() && !std::isspace(ch)) _input_stream->unget(); // Put back the non-digit character if it's not a space or EOF
        return ConstantToken{value * sign};
      }  
      throw std::runtime_error("Unexpected character: " + std::string(1, ch));
  }
}

