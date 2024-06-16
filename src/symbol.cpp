#include <beescript/symbol.h>

#include <beescript/errors.h>

#include <stdexcept>

namespace bees {
std::string getSymbolTypeName(SymbolType type)
{
    switch(type)
    {
    case SymbolType::Local:
        return "Local";
    case SymbolType::Global:
        return "Global";
    case SymbolType::Param:
        return "Param";
    default:
        throw BeeError("Unknown SymbolType");
    }
}
} //namespace bees