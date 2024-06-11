#include <symbol-table.h>

#include <stdexcept>

namespace bees {

SymbolTable::SymbolTable()
{
    enterScope();
}

void SymbolTable::enterScope()
{
    // Put front so we can use a forward iterator
    scopes.emplace_front();
}

void SymbolTable::exitScope()
{
    scopes.pop_front();
}

int SymbolTable::level()
{
    return scopes.size();
}

void SymbolTable::bind(SymbolPtr symbol)
{
    scopes.front()[symbol->name] = symbol;
}

SymbolPtr SymbolTable::lookup(const std::string& name)
{
    for(const auto& scope : scopes)
    {
        try
        {
            return scope.at(name);
        }
        catch(const std::out_of_range&)
        {
            // pass
        }
    }

    return nullptr;
}

SymbolPtr SymbolTable::lookupLocal(const std::string& name)
{
    try
    {
        return scopes.front().at(name);
    }
    catch(const std::out_of_range&)
    {
        return nullptr;
    }
}

void SymbolTable::resolveDecl(DeclNodePtr node)
{
    if(!node)
    {
        return;
    }

    auto out = std::make_shared<Symbol>();
    out->kind = level() > 1 ? SymbolType::Local : SymbolType::Global;
    out->name = node->name;

    resolveExpr(node->value);
    bind(out);

    if(node->code)
    {
        enterScope();
        resolveArgList(node->type->args);
        resolveStmt(node->code);
        exitScope();
    }

    resolveNext(node);
}

void SymbolTable::resolveStmt(StmtNodePtr node)
{
    switch(node->kind)
    {
    case StmtType::Decl:
        resolveDecl(node->decl);
        break;
    case StmtType::Expr:
        resolveExpr(node->expr);
        break;
    case StmtType::For:
        enterScope();
        resolveExpr(node->initExpr);
        resolveExpr(node->expr);
        resolveExpr(node->nextExpr);
        resolveStmt(node->body);
        exitScope();
        break;
    case StmtType::IfElse:
        resolveExpr(node->expr);
        enterScope();
        resolveStmt(node->body);
        exitScope();
        resolveStmt(node->elseBody);
        break;
    case StmtType::Return:
        resolveExpr(node->expr);
        break;
    case StmtType::Block:
        enterScope();
        resolveStmt(node->body);
        exitScope();
        break;
    default:
        throw std::runtime_error("Invalid StmtType");
    }

    resolveNext(node);
}

void SymbolTable::resolveNext(SyntaxNodePtr node)
{
    if(node->next)
    {
        // TODO fix this, can't dynamic cast because SyntaxNode
        // doesn't have any virtual funcs and is therefore not polymorphic
        auto nextDecl = std::dynamic_pointer_cast<DeclNode>(node->next);
        if(nextDecl)
        {
            resolveDecl(nextDecl);
            return;
        }

        auto nextStmt = std::dynamic_pointer_cast<StmtNode>(node->next);
        if(nextStmt)
        {
            resolveStmt(nextStmt);
            return;
        }
    }
}

void SymbolTable::resolveExpr(ExprNodePtr node)
{
    if(!node)
    {
        return;
    }

    if(node->type == ExprType::Name)
    {
        node->symbol = lookup(node->name);
    }
    else
    {
        resolveExpr(node->left);
        resolveExpr(node->right);
    }
}

} //namespace bees