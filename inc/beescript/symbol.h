#pragma once

#include <memory>
#include <string>

namespace bees {

enum class SymbolType
{
    Local,
    Global,
    Param
};

std::string getSymbolTypeName(SymbolType type);

class TypeNode;

class Symbol
{
public:
    SymbolType kind;
    std::shared_ptr<TypeNode> type;
    std::string name;
    int which;
};

using SymbolPtr = std::shared_ptr<Symbol>;

} //namespace bees