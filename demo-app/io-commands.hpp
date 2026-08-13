#ifndef IO_COMMANDS_HPP
#define IO_COMMANDS_HPP

#include <iostream>

#include <tecslog/tecslog.hpp>

#include "log-info.hpp"
#include "thread-safe-queue.hpp"

namespace demo
{
  std::error_code silenceCommand(std::istream& in, std::ostream& out);
  std::error_code defaultCommand(std::istream& in, std::ostream& out);
  std::error_code logCommand(std::istream& in, std::ostream& out, LogInfo& info, ThreadSafeQueue< LogInfo >& queue);
  std::error_code helpCommand(std::istream& in, std::ostream& out);
}

#endif
