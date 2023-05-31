#ifndef  KEI_FRONTEND_TOKENIZER_H
#define KEI_FRONTEND_TOKENIZER_H


#include <variant>
#include <optional>
#include <istream>

struct SymbolToken {
    std::string name;
    bool operator == (const SymbolToken& other) const;
};

struct QuoteToken {
    bool operator == (const QuoteToken& ) const;
};

struct DotToken {
    bool operator == (const DotToken& ) const;
};

enum class BracketToken {
    LEFT, RIGHT
};

struct ConstantToken {
    int value;
    bool operator == (const ConstantToken& other) const;
};

typedef std::variant<
    ConstantToken,
    BracketToken,
    SymbolToken,
    QuoteToken,
    DotToken>
    Token;

class Tokenizer {
 public:
  Tokenizer(std::istream* input);
  bool IsEnd();
  void Next();
  Token GetToken();
 private:
  std::istream* _input_stream;
  bool _is_end = false;
};
#endif