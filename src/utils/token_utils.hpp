#ifndef TOKEN_UTILS_HPP
#define TOKEN_UTILS_HPP

#include <ostream>

#include "../tokens/token.hpp"
#include "variant_utils.hpp"

namespace tokens {

inline std::ostream &
operator<<(std::ostream &out, Token const &token) {
#define X(SymbolName, Char)                                                                                            \
	case TokenType::SymbolName: out << "Symbol::" #SymbolName; break;

	switch (token.type) {
	case TokenType::Identifier:
		out << token.name;
		break;

		XSYMBOLS
	}

#undef X
	return out;
}

} // namespace tokens

#endif