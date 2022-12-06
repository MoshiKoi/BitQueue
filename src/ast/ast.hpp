#pragma once
#ifndef AST_HPP
#define AST_HPP

#include <memory>
#include <string>
#include <variant>
#include <vector>

namespace ast {

struct EnqueueConst {
	unsigned value;
};
struct EnqueueInput {};
struct DequeueOutput {};
struct Call {
	std::string funcName;
};
struct AnonymousCall;
struct Return {};
struct ReturnTop {};
struct Recurse {};
struct Conditional;
struct Block;
struct Define;

using Stmt = std::variant<EnqueueConst, EnqueueInput, DequeueOutput, Call, AnonymousCall, Return, ReturnTop, Recurse,
                          Conditional, Block, Define>;

struct Define {
	std::string funcName;
	std::unique_ptr<Stmt> body;
};

struct AnonymousCall {
	std::unique_ptr<Stmt> body;
};

struct Block {
	std::vector<Stmt> body;
};

struct Conditional {
	std::unique_ptr<Stmt> body;
};

} // namespace ast

#endif
