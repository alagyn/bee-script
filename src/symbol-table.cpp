#include <beescript/symbol-table.h>

#include <beescript/errors.h>

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

    resolve(node->next);
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
        throw BeeError("Invalid StmtType: " + getStmtTypeName(node->kind));
    }

    resolve(node->next);
}

void SymbolTable::resolve(SyntaxNodePtr node)
{
    if(!node)
    {
        return;
    }
    switch(node->nodeType)
    {
    case NodeType::Decl:
        resolveDecl(std::static_pointer_cast<DeclNode>(node));
        break;
    case NodeType::Stmt:
        resolveStmt(std::static_pointer_cast<StmtNode>(node));
        break;
    case NodeType::Expr:
        resolveExpr(std::static_pointer_cast<ExprNode>(node));
        break;
    default:
        throw BeeError("Invalid NodeType: " + getNodeTypeName(node->nodeType));
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

void SymbolTable::resolveArgList(ArgListPtr args, int index)
{
    if(!args)
    {
        return;
    }

    auto out = std::make_shared<Symbol>();
    out->kind = SymbolType::Param;
    out->name = args->name;
    out->which = index;

    bind(out);
    args->symbol = out;

    resolveArgList(std::static_pointer_cast<ArgList>(args->next), index + 1);
}

} //namespace bees