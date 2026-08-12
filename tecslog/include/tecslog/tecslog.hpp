#ifndef TECSLOG_HPP
#define TECSLOG_HPP

#include <optional>
#include <string>
#include <system_error>

#include <tecslog/types.hpp>

namespace tecslog
{
  std::error_code init(const std::string& filename, Level level);
  std::error_code init(const std::string& filename, const std::string& level);

  std::error_code setLevel(Level level);
  std::error_code setLevel(const std::string& level);
  std::ostream& printPossibleLevels(std::ostream& out);
  std::optional< Level > parseLevel(const std::string& str_level);
  Level getDefaultLevel();
  std::string getLogFile();
  void reset();

  std::error_code info(const std::string& message);
  std::error_code warning(const std::string& message);
  std::error_code error(const std::string& message);

  std::error_code log(Level level, const std::string& message);
  std::error_code log(const std::string& level, const std::string& message);
}

#endif
