#include "parser.hpp"
#include "ast.hpp"

#include "../utils/variant_utils.hpp"

#include <memory>
#include <stdexcept>
#include <type_traits>
#include <variant>
#include <vector>

namespace ast {

Call
parseCall(tokens::Tokens::iterator &begin, tokens::Tokens::iterator const &end) {
	++begin;

	if ((*begin).type != tokens::TokenType::Identifier)
		throw std::runtime_error{"Expected a function name"};

	auto name = (*begin).name;
	++begin;
	return Call{name};
}

Block
parseBlock(tokens::Tokens::iterator &begin, tokens::Tokens::iterator const &end) {
	++begin;
	auto retval = Block{parseStmts(begin, end)};
	if ((*begin).type != tokens::TokenType::RightParen)
		throw std::runtime_error{"Expected closing parenthesis"};
	++begin;
	return retval;
}

Define
parseDefinition(tokens::Tokens::iterator &begin, tokens::Tokens::iterator const &end) {
	++begin;
	if ((*begin).type != tokens::TokenType::Identifier)
		throw std::runtime_error{"Expected a function name"};

	auto name = (*begin).name;
	++begin;
	return Define{.funcName = name, .body = std::make_unique<Stmt>(parseStmt(begin, end))};
}

Conditional
parseConditional(tokens::Tokens::iterator &begin, tokens::Tokens::iterator const &end) {
	++begin;
	return Conditional{.body = std::make_unique<Stmt>(parseStmt(begin, end))};
}

AnonymousCall
parseAnonymous(tokens::Tokens::iterator &begin, tokens::Tokens::iterator const &end) {
	++begin;
	return AnonymousCall{.body = std::make_unique<Stmt>(parseStmt(begin, end))};
}

Stmt
parseStmt(tokens::Tokens::iterator &begin, tokens::Tokens::iterator const &end) {

#define SIMPLE(Command)                                                                                                \
	++begin;                                                                                                           \
	return Command;

	switch ((*begin).type) {
	case tokens::TokenType::Zero: SIMPLE(EnqueueConst{0})
	case tokens::TokenType::One: SIMPLE(EnqueueConst{1})
	case tokens::TokenType::RightAngle: return parseCall(begin, end);
	case tokens::TokenType::LeftAngle: SIMPLE(Return{})
	case tokens::TokenType::Caret: SIMPLE(ReturnTop{})
	case tokens::TokenType::Quote: SIMPLE(Recurse{})
	case tokens::TokenType::Colon: return parseDefinition(begin, end);
	case tokens::TokenType::Apostrophe: return parseAnonymous(begin, end);
	case tokens::TokenType::Question: return parseConditional(begin, end);
	case tokens::TokenType::LeftParen: return parseBlock(begin, end);
	case tokens::TokenType::RightParen: throw std::runtime_error{"Closing parenthesis without matching opening"};
	case tokens::TokenType::Comma: SIMPLE(EnqueueInput{})
	case tokens::TokenType::Dot: SIMPLE(DequeueOutput{})
	case tokens::TokenType::Identifier: throw std::runtime_error{"Command must start with a symbol"};
	}
#undef SIMPLE
}

std::vector<Stmt>
parseStmts(tokens::Tokens::iterator &begin, tokens::Tokens::iterator const &end) {
	std::vector<Stmt> stmts{};
	while (begin != end) {
		if ((*begin).type == tokens::TokenType::RightParen) {
			break;
		}
		stmts.push_back(parseStmt(begin, end));
	}
	return stmts;
}

} // namespace ast