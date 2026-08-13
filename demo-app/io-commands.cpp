#include "io-commands.hpp"

#include <iomanip>
#include <sstream>

namespace
{
  std::error_code readWordTillEnd(std::istream& in, std::string& str)
  {
    std::string buffer;
    std::getline(in, buffer);
    std::stringstream sin(buffer);

    if (!(sin >> str))
    {
      return std::make_error_code(std::errc::io_error);
    }
    return {};
  }
}

std::error_code demo::silenceCommand(std::istream& in, std::ostream& out)
{
  std::string str_level;
  std::error_code code = readWordTillEnd(in, str_level);
  if (code)
  {
    return code;
  }

  auto level = tecslog::parseLevel(str_level);
  if (!level.has_value())
  {
    return std::make_error_code(std::errc::invalid_argument);
  }
  LogInfo::silence_level = level.value();
  out << "OK: silence level is specified on " << level.value();
  return {};
}

std::error_code demo::defaultCommand(std::istream& in, std::ostream& out)
{
  std::string str_level;
  std::error_code code = readWordTillEnd(in, str_level);
  if (code)
  {
    return code;
  }

  auto level = tecslog::parseLevel(str_level);
  if (!level.has_value())
  {
    return std::make_error_code(std::errc::invalid_argument);
  }
  tecslog::setLevel(level.value());
  out << "OK: default level is specified on " << level.value();
  return {};
}

std::error_code demo::logCommand(std::istream& in, std::ostream& out, LogInfo& info, ThreadSafeQueue< LogInfo >& queue)
{
  in >> std::quoted(info.message);
  if (!in)
  {
    return std::make_error_code(std::errc::io_error);
  }

  std::string str_level;
  std::error_code code = readWordTillEnd(in, str_level);
  if (code && !str_level.empty())
  {
    return code;
  }

  auto level = tecslog::parseLevel(str_level); 
  if (level.has_value())
  {
    info.level = level.value();
  }
  else if (LogInfo::silence_level.has_value())
  {
    info.level = LogInfo::silence_level.value();
  }
  else
  {
    return std::make_error_code(std::errc::invalid_argument);
  }
  queue.push(info);
  out << "OK: log with message: \"" << info.message << "\" and level: " << info.level;
  return {};
}

std::error_code demo::helpCommand(std::istream& in, std::ostream& out)
{
  out << "Commands:\n"
    << "\t1 default <level>\t= Specify default log level, logs with a lower level will not be recorded\n"
    << "\t2 silence <level>\t= Specify silence log level\n"
    << "\t3 log <message> [level]\t= Log message with optional level\n"
    << "\tThe <level> may not be specified if it was specified by the `silence` command.\n"
    << "\tIn such case log will have <level>, specified as `silence` <level>\n"
    << "\tIf you did not specify <level> and not use `silence`, command is invalid\n\n";

  out << "Log levels:\n";
  tecslog::printPossibleLevels(out);

  return {};
}
