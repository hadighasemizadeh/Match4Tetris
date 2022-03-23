#ifndef REDERER_EXCEPTION_HANDLER_HPP
#define REDERER_EXCEPTION_HANDLER_HPP

#include <exception>
#include <string>

namespace MatchThree 
{
    namespace View
    {
        struct RendererExceptionHandler : public std::exception
        {
            std::string error;
            explicit RendererExceptionHandler(const std::string error):error(error){}
        };
    }
}
#endif