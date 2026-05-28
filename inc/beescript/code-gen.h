#pragma once

#include <beescript/syntax-node.h>

#include <fstream>

namespace bees {

class CodeGenerator
{
public:
    CodeGenerator(const std::string& filename);
    ~CodeGenerator();

    void generate(SyntaxNodePtr root);

protected:
    virtual int allocRegister() = 0;
    virtual void freeRegister(int reg) = 0;
    virtual std::string registerName(int reg) = 0;

    virtual int createLabel() = 0;
    virtual std::string labelName(int label) = 0;

private:
    std::ofstream out;

    void genDecl(DeclNodePtr node);
    void genStmt(StmtNodePtr node);
    void genExpr(ExprNodePtr node);
    void genSymbol(SymbolPtr symbol);
};

} //namespace bees