#include <tecslog/tecslog.hpp>

#include "logger.hpp"

std::error_code tecslog::init(const std::string& filename, Level level)
{
  Logger& logger = Logger::instance();
  std::error_code code = logger.setFile(filename);
  if (code)
  {
    return code;
  }
  return logger.setLevel(level);
}

std::error_code tecslog::init(const std::string& filename, const std::string& level)
{
  Logger& logger = Logger::instance();
  std::error_code code = logger.setFile(filename);
  if (code)
  {
    return code;
  }
  return logger.setLevel(level);
}

std::error_code tecslog::setLevel(Level level)
{
  Logger& logger = Logger::instance();
  return logger.setLevel(level);
}

std::error_code tecslog::setLevel(const std::string& level)
{
  Logger& logger = Logger::instance();
  return logger.setLevel(level);
}

std::ostream& tecslog::printPossibleLevels(std::ostream& out)
{
  out << 0 << ' ' << Logger::levelToStr(Level::MIN);
  for (int i = static_cast< int >(Level::MIN) + 1; i <= static_cast< int >(Level::MAX); i++)
  {
    out << '\n' << i << ' ' << Logger::levelToStr(static_cast< Level >(i));
  }
  return out;
}

std::optional< tecslog::Level > tecslog::parseLevel(const std::string& str_level)
{
  return Logger::strToLevel(str_level);
}

tecslog::Level tecslog::getDefaultLevel()
{
  Logger& logger = Logger::instance();
  return logger.getDefaultLevel();
}

std::string tecslog::getLogFile()
{
  Logger& logger = Logger::instance();
  return logger.getLogFile();
}

void tecslog::reset()
{
  Logger& logger = Logger::instance();
  return logger.reset();
}

std::error_code tecslog::info(const std::string& message)
{
  Logger& logger = Logger::instance();
  return logger.info(message);
}

std::error_code tecslog::warning(const std::string& message)
{
  Logger& logger = Logger::instance();
  return logger.warning(message);
}

std::error_code tecslog::error(const std::string& message)
{
  Logger& logger = Logger::instance();
  return logger.error(message);
}

std::error_code tecslog::log(Level level, const std::string& message)
{
  Logger& logger = Logger::instance();
  return logger.log(level, message);
}

std::error_code tecslog::log(const std::string& level, const std::string& message)
{
  Logger& logger = Logger::instance();
  return logger.log(level, message);
}
