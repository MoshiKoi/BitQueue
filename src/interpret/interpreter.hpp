#pragma once
#ifndef INTERPRETER_HPP
#define INTERPRETER_HPP

#include <queue>
#include <stack>
#include <string>
#include <unordered_map>
#include <variant>
#include <vector>

#include "../ast/ast.hpp"

class Interpreter {
	std::queue<unsigned> queue;
	std::unordered_map<std::string, ast::Stmt const &> functions;
	std::stack<ast::Stmt const *> callStack;
	 
	size_t const callStackLimit = 1000;

	void call(ast::Stmt const &);
	void uncall();
  public:
	Interpreter() : queue{}, functions{}, callStack{} {}
	void run(std::vector<ast::Stmt> const &stmts) {
		for (auto &stmt : stmts) {
			std::visit(*this, stmt);
		}
	};

	void operator()(ast::EnqueueConst const &);
	void operator()(ast::EnqueueInput const &);
	void operator()(ast::DequeueOutput const &);
	void operator()(ast::Call const &);
	void operator()(ast::AnonymousCall const &);
	void operator()(ast::Return const &);
	void operator()(ast::ReturnTop const &);
	void operator()(ast::Recurse const &);
	void operator()(ast::Conditional const &);
	void operator()(ast::Block const &);
	void operator()(ast::Define const &);
};

#endif