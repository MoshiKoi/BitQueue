#include "tokenizer.hpp"
#include "token.hpp"

#include <exception>
#include <iterator>
#include <sstream>
#include <stdexcept>

namespace tokens {

UnexpectedCharacterError::UnexpectedCharacterError(char chr, size_t line, size_t col) : chr(chr), line(line), col(col) {
	std::stringstream builder{};
	builder << "Unexpected character `" << chr << "`at line " << line << " column " << col << std::endl;
	msg = std::string{builder.str()};
}
const char *
UnexpectedCharacterError::what() const noexcept {
	return msg.c_str();
}

Tokens::iterator::iterator(std::string::const_iterator begin, std::string::const_iterator end,
                           std::string::const_iterator cur)
    : _begin{begin}, _end{end}, _cur{cur}, _curLine{1}, _curCol{0} {
	goToNextToken();

	// Get the first token, if it exists, otherwise immediately end
	_isEnded = _cur == _end;
	if (!_isEnded) {
		operator++();
	}
}

void
Tokens::iterator::goToNextToken() {
	while (_cur != _end) {
		switch (*_cur) {
			// Whitespace handling
		case '\n':
			++_cur;
			++_curLine;
			_curCol = 0;
			continue;
		case ' ':
		case '\r':
		case '\t':
			++_cur;
			++_curCol;
			continue;
			// Comments
		case ';':
			while (_cur != _end && *_cur != '\n')
				++_cur;
			continue;
		default: goto end;
		}
	}
end:
	return;
}

Tokens::iterator &
Tokens::iterator::operator++() {
	if (_cur == _end) {
		this->_isEnded = true;
		return *this;
	}

	switch (*_cur) {
		// Single character tokens
#define X(SymbolName, Char)                                                                                            \
	case static_cast<char>(Char):                                                                                      \
		++_cur;                                                                                                        \
		++_curCol;                                                                                                     \
		_currentToken = {.type = TokenType::SymbolName, .line = _curLine, .column = _curCol};                          \
		break;

		XSYMBOLS

#undef X

	// Identifiers
	// switch range extension
	case 'a' ... 'z':
	case 'A' ... 'Z': {
		auto start = _cur;
		while (_cur != _end &&
		       ((*_cur >= 'a' && *_cur <= 'z') || (*_cur >= 'A' && *_cur <= 'Z') || (*_cur >= '0' && *_cur <= '9'))) {
			++_cur;
		}
		_currentToken = {
		    .type = TokenType::Identifier, .name = std::string{start, _cur}, .line = _curLine, .column = _curCol};
		break;
	}
	default: throw UnexpectedCharacterError(*_cur, _curLine, _curCol);
	}
	goToNextToken();
	return *this;
}

} // namespace tokens