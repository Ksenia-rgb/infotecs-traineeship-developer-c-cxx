#ifndef LOG_INFO_HPP
#define LOG_INFO_HPP

#include <optional>
#include <string>

#include <tecslog/tecslog.hpp>

namespace demo
{
  struct LogInfo
  {
    std::string message;
    tecslog::Level level;
    static std::optional< tecslog::Level > silence_level;
  };
}

#endif
