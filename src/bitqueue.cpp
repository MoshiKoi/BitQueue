#include <algorithm>
#include <fstream>
#include <iostream>
#include <iterator>
#include <sstream>
#include <variant>

#include "ast/ast.hpp"
#include "ast/parser.hpp"
#include "interpret/interpreter.hpp"
#include "tokens/tokenizer.hpp"
#include "utils/token_utils.hpp"
#include "utils/variant_utils.hpp"

void
printStmt(ast::Stmt const &stmt, int indentLevel = 0) {
	for (int i = 0; i < indentLevel; ++i)
		std::cout << "  ";

	std::visit(overloaded{
	               [](ast::EnqueueConst const &node) { std::cout << "EnqueueConst(" << node.value << ")\n"; },
	               [](ast::EnqueueInput const &node) { std::cout << "EnqueueInput" << '\n'; },
	               [](ast::DequeueOutput const &node) { std::cout << "Dequeue" << '\n'; },
	               [](ast::Call const &node) { std::cout << "Call " << node.funcName << '\n'; },
	               [&](ast::AnonymousCall const &node) {
		               std::cout << "Anonymous\n";
		               printStmt(*node.body, indentLevel + 1);
	               },
	               [](ast::Return const &node) { std::cout << "Return" << '\n'; },
	               [](ast::ReturnTop const &node) { std::cout << "ReturnTop" << '\n'; },
	               [](ast::Recurse const &node) { std::cout << "Recurse" << '\n'; },
	               [&](ast::Conditional const &node) {
		               std::cout << "Conditional\n";
		               printStmt(*node.body, indentLevel + 1);
	               },
	               [&](ast::Block const &node) {
		               std::cout << "do\n";
		               for (auto &stmt : node.body)
			               printStmt(stmt, indentLevel + 1);
	               },
	               [&](ast::Define const &node) {
		               std::cout << "def " << node.funcName << "\n";
		               printStmt(*node.body, indentLevel + 1);
	               },
	           },
	           stmt);
}

int
main(int argc, char const **args) {
	if (argc < 2) {
		std::cerr << "Please enter file name" << std::endl;
		return 1;
	}
	bool const emitSource = std::find(args, args + argc, std::string{"--emitSource"}) != args + argc;
	bool const emitTokens = std::find(args, args + argc, std::string{"--emitTokens"}) != args + argc;
	bool const emitParse = std::find(args, args + argc, std::string{"--emitTree"}) != args + argc;

	char const *filePath = args[1];
	std::ifstream ifs{filePath};
	std::string source{std::istreambuf_iterator<char>{ifs}, {}};

	if (emitSource) {
		std::cout << "===== Source =====\n" << source << '\n';
	}

	tokens::Tokens toks{source};

	if (emitTokens) {
		std::cout << "===== Tokens =====\n";

		for (auto token : toks) {
			std::cout << filePath << ':' << token.line << ':' << token.column << ' ' << token << '\n';
		}
	}

	auto begin = toks.begin();
	auto stmts = ast::parseStmts(begin, toks.end());

	if (emitParse) {
		std::cout << "===== Explanation ====\n";
		for (auto &stmt : stmts) {
			printStmt(stmt);
		}
	}

	// Emit the separator only if there is something to separate it from
	if (emitSource || emitTokens || emitParse) {
		std::cout << "===== Output =====\n";
	}
	Interpreter interpreter{};
	interpreter.run(std::move(stmts));
}