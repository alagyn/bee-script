#pragma once
#include <beescript/syntax-node.h>

#include <deque>
#include <map>

namespace bees {

class SymbolTable
{
public:
    SymbolTable();

    // Push a new scope onto the stack
    void enterScope();
    // Pop a scope off the stack
    void exitScope();
    // Current stack depth
    int level();
    // Bind name to current scope
    void bind(SymbolPtr symbol);
    // Lookup symbol in the current scope, and then each higher scope
    SymbolPtr lookup(const std::string& name);
    // Lookup symbol ONLY in the current scope
    SymbolPtr lookupLocal(const std::string& name);

    void resolve(SyntaxNodePtr node);

private:
    void resolveDecl(DeclNodePtr node);
    void resolveStmt(StmtNodePtr node);
    void resolveExpr(ExprNodePtr node);
    void resolveArgList(ArgListPtr args, int index = 0);

private:
    std::deque<std::map<std::string, SymbolPtr>> scopes;
};

using SymbolTablePtr = std::shared_ptr<SymbolTable>;

} //namespace bees
