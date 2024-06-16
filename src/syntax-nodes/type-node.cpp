#include <beescript/syntax-node.h>

#include <beescript/errors.h>

#include <sstream>
#include <stdexcept>

namespace bees {

std::string getPrimitiveTypeName(PrimitiveType type)
{
    switch(type)
    {
    case PrimitiveType::Void:
        return "Void";
    case PrimitiveType::Byte:
        return "Byte";
    case PrimitiveType::Int:
        return "Int";
    case PrimitiveType::Str:
        return "Str";
    case PrimitiveType::Bool:
        return "Bool";
    case PrimitiveType::Func:
        return "Func";
    case PrimitiveType::Array:
        return "Array";
    default:
        std::stringstream ss;
        ss << "Unknown PrimitiveType (" << static_cast<unsigned>(type) << ")";
        throw BeeError(ss.str());
    }
}

ArgList::ArgList()
    : SyntaxNode(NodeType::ArgList)
{
}

TypeNode::TypeNode()
    : SyntaxNode(NodeType::Type)
{
}

bool TypeNode::equals(const TypeNodePtr other)
{
    return equals(*other);
}

bool TypeNode::equals(const TypeNode& other)
{
    if(primType != other.primType)
    {
        return false;
    }

    switch(primType)
    {
    case PrimitiveType::Array:
        return subtype->equals(*other.subtype);
    case PrimitiveType::Func:
        // Check return type
        if(subtype->equals(*other.subtype))
        {
            ArgListPtr temp1 = args;
            ArgListPtr temp2 = other.args;

            while(temp1 && temp2)
            {
                if(!temp1->type->equals(temp2->type))
                {
                    return false;
                }

                temp1 = std::static_pointer_cast<ArgList>(temp1->next);
                temp2 = std::static_pointer_cast<ArgList>(temp2->next);
            }

            // Check for one being null and the other not
            if((bool)temp1 != (bool)temp2)
            {
                return false;
            }

            return true;
        }

        return false;
    default:
        // No subtypes, types are equal
        return true;
    }
}

std::string TypeNode::toStr()
{
    std::stringstream ss;
    switch(primType)
    {
    case PrimitiveType::Array:
        ss << subtype->toStr() << "[" << size << "]";
        break;
    case PrimitiveType::Func:
    {
        ss << "Func[";
        ArgListPtr temp = args;
        while(temp)
        {
            ss << temp->type->toStr();
            temp = std::static_pointer_cast<ArgList>(temp->next);
            if(temp)
            {
                ss << ", ";
            }
        }
        ss << "] -> " << subtype->toStr();
    }
    default:
        ss << getPrimitiveTypeName(primType);
    }

    return ss.str();
}

std::string ArgList::toStr()
{
    return "NotImpl";
}

} //namespace bees