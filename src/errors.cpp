#include <beescript/errors.h>

namespace bees {

BeeError::BeeError(const std::string& msg)
    : exception()
    , msg(msg)

{
}

const char* BeeError::what() const noexcept
{
    return msg.c_str();
}

} //namespace bees