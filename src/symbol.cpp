#include <symbol.h>

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
        throw std::runtime_error("Unknown SymbolType");
    }
}
} //namespace bees