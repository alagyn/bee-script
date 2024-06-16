#pragma once

#include <exception>
#include <string>

namespace bees {

class BeeError : public std::exception
{
public:
    BeeError(const std::string& msg);

    const char* what() const noexcept override;

private:
    std::string msg;
};

} //namespace bees