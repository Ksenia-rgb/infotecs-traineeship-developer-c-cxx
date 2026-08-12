#ifndef LOGGER_HPP
#define LOGGER_HPP

#include <fstream>
#include <mutex>
#include <optional>
#include <string>
#include <unordered_map>

#include <tecslog/types.hpp>

namespace tecslog
{
  class Logger
  {
  public:
    static Logger& instance();

    std::error_code setFile(const std::string& filename);
    bool isFileOpen();
    std::error_code setLevel(Level level);
    std::error_code setLevel(const std::string& str_level);
    Level getDefaultLevel();
    std::string getLogFile();
    void reset();

    std::error_code info(const std::string& message);
    std::error_code warning(const std::string& message);
    std::error_code error(const std::string& message);
    std::error_code log(Level level, const std::string& message);
    std::error_code log(const std::string& str_level, const std::string& message);

    static std::string levelToStr(Level level);
    static std::optional< Level > strToLevel(const std::string& level);
  private:
    static const std::string default_filename_;
    static const Level default_level_;
    static const std::unordered_map< tecslog::Level, std::string > level_map;
    static const std::unordered_map< std::string, tecslog::Level > str_level_map;

    std::string config_filename_;
    std::string open_filename_;
    Level level_;
    std::ofstream out_;

    std::mutex mutex_level;
    std::mutex mutex_file;

    Logger();
    ~Logger();
    Logger(const Logger&) = delete;
    Logger& operator=(const Logger&) = delete;

    std::error_code uncheckLevelLog(const std::string& str_level, const std::string& message);
    std::error_code ensureFileOpen();
  };
}

#endif
