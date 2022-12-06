#include "../tokens/tokenizer.hpp"
#include "ast.hpp"
#include <vector>

namespace ast {

Stmt parseStmt(tokens::Tokens::iterator &begin, tokens::Tokens::iterator const &end);
std::vector<Stmt> parseStmts(tokens::Tokens::iterator &begin, tokens::Tokens::iterator const &end);

} // namespace ast