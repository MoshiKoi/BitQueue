#include "interpreter.hpp"
#include "../ast/ast.hpp"

#include <cstdint>
#include <iostream>
#include <stdexcept>
#include <variant>

struct ReturnSignal {};
struct ReturnTopSignal {};
struct RequestRerunSignal {};

void
Interpreter::call(ast::Stmt const &body) {
	if (callStack.size() > callStackLimit)
		throw std::runtime_error{"Exceded the call stack limit"};

	callStack.push(&body);
	std::visit(*this, body);
}

void
Interpreter::uncall() {
	if (callStack.size() == 0)
		throw std::runtime_error{"Can't return from top level"};
	callStack.pop();
}

void
Interpreter::operator()(ast::EnqueueConst const &node) {
	queue.push(node.value);
}
void
Interpreter::operator()(ast::EnqueueInput const &) {
	auto inp = std::cin.get();

	if (inp != std::char_traits<decltype(std::cin)>::eof()) {
		for (int i = 0; i < 8; ++i)
			queue.push((inp >> (7 - i)) & 0x1);
	}
}
void
Interpreter::operator()(ast::DequeueOutput const &) {
	std::uint8_t res = 0;
	for (int i = 0; i < 8; ++i) {
		res <<= 1;
		res |= queue.front();
		queue.pop();
	}
	std::cout << static_cast<char>(res) << std::flush;
}
void
Interpreter::operator()(ast::Call const &node) {
	auto func = functions.find(node.funcName);
	if (func == functions.end()) {
		throw std::runtime_error(std::string{"Undefined symbol "} + node.funcName);
	}

call:
	try {
		call(func->second);
	} catch (ReturnSignal) {
		// Continue
	} catch (ReturnTopSignal) {
		// Re-call the function
		// We can't really recall ourselves, so we request the caller to recall us
		uncall();
		throw RequestRerunSignal{};
	} catch (RequestRerunSignal) { goto call; }
}

void
Interpreter::operator()(ast::AnonymousCall const &node) {
	call(*node.body);
}

void
Interpreter::operator()(ast::Return const &) {
	uncall();
	throw ReturnSignal{};
}

void
Interpreter::operator()(ast::ReturnTop const &) {
	uncall();
	throw ReturnTopSignal{};
}

void
Interpreter::operator()(ast::Recurse const &) {
	call(*callStack.top());
}
void
Interpreter::operator()(ast::Conditional const &node) {
	auto cond = queue.front();
	queue.pop();
	if (cond) {
		std::visit(*this, *node.body);
	}
}
void
Interpreter::operator()(ast::Block const &node) {
	run(node.body);
}
void
Interpreter::operator()(ast::Define const &node) {
	functions.insert({node.funcName, *(node.body)});
}