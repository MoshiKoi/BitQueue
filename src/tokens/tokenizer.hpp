#ifndef TOKENIZER_HPP
#define TOKENIZER_HPP

#include "token.hpp"

#include <cstddef>
#include <optional>
#include <string>

namespace tokens {

class UnexpectedCharacterError : std::exception {
  public:
	char const chr;
	size_t const line, col;
	std::string msg;
	UnexpectedCharacterError(char chr, size_t line, size_t col);
	const char *what() const noexcept override;
};

class Tokens {
	std::string const &_source;

  public:
	Tokens(std::string const &source) : _source(source) {}

	class iterator {
		using InnerIterator = std::string::const_iterator;

	  private:
		InnerIterator const _begin; // Beginning of source
		InnerIterator const _end;   // End of source
		InnerIterator _cur;         // Current iteration position in the source
		bool _isEnded;              // Whether the iterator has reached the end
		std::size_t _curLine;
		std::size_t _curCol;
		Token _currentToken;

		void goToNextToken();

	  public:
		iterator(InnerIterator begin, InnerIterator end, InnerIterator cur);
		iterator &operator++();
		iterator operator++(int) {
			iterator retval{*this};
			++(*this);
			return retval;
		};
		bool operator==(iterator other) const { return this->_cur == other._cur && this->_isEnded == other._isEnded; }
		bool operator!=(iterator other) const { return !(*this == other); }
		Token operator*() const { return _currentToken; }

		// iterator traits
		using difference_type = std::size_t;
		using value_type = std::size_t;
		using pointer = Token const *;
		using reference = Token const &;
		using iterator_category = std::forward_iterator_tag;
	};
	iterator begin() { return iterator(_source.begin(), _source.end(), _source.begin()); }
	iterator end() { return iterator(_source.begin(), _source.end(), _source.end()); }
};

} // namespace tokens

#endif