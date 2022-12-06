#ifndef TOKEN_HPP
#define TOKEN_HPP

#include <cstddef>
#include <string>
#include <variant>

namespace tokens {

#define XSYMBOLS                                                                                                       \
	X(One, '1')                                                                                                        \
	X(Zero, '0')                                                                                                       \
	X(LeftAngle, '<')                                                                                                  \
	X(RightAngle, '>')                                                                                                 \
	X(Caret, '^')                                                                                                      \
	X(Quote, '"')                                                                                                      \
	X(Colon, ':')                                                                                                      \
	X(Apostrophe, '\'')                                                                                                \
	X(Question, '?')                                                                                                   \
	X(Comma, ',')                                                                                                      \
	X(Dot, '.')                                                                                                        \
	X(LeftParen, '(')                                                                                                  \
	X(RightParen, ')')

#define X(SymbolName, Char) SymbolName,

enum class TokenType {
	Identifier,
	XSYMBOLS
};

#undef X



struct Token {
	TokenType type;
	std::string name;
	std::size_t line;
	std::size_t column;
};

} // namespace tokens

#endif