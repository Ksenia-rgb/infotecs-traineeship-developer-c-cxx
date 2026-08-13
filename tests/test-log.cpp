#include <fstream>
#include <sstream>

#include <tecslog/tecslog.hpp>

#include "test-runner.hpp"

TEST_CASE(TecslogCorrectInfoWarnErrorTest)
{
  tecslog::reset();

  std::string filename = "test.log";
  test::TestFileGuard guard(filename);

  std::error_code code_init = tecslog::init(filename, tecslog::Level::INFO);
  std::error_code code = tecslog::info("Info mess");
  code = tecslog::warning("Warn mess");
  code = tecslog::error("Error mess");

  std::ifstream file(filename);
  std::ostringstream sout;
  sout << file.rdbuf();

  bool info_found = sout.str().find("[INFO] Info mess") != std::string::npos;
  bool warn_found = sout.str().find("[WARNING] Warn mess") != std::string::npos;
  bool error_found = sout.str().find("[ERROR] Error mess") != std::string::npos;

  return code_init.value() == 0 && code.value() == 0 && info_found && warn_found && error_found;
}

TEST_CASE(TecslogErrorWithIncorrectFileTest)
{
  tecslog::reset();

  std::string filename_unreal = "logs/unreal-directory/test.log";
  std::string filename_default = tecslog::getLogFile();
  test::TestFileGuard guard_unreal(filename_unreal);
  test::TestFileGuard guard(filename_default);

  std::error_code code_init = tecslog::init(filename_unreal, tecslog::Level::WARNING);
  std::error_code code = tecslog::error("Error mess");

  std::ifstream file(filename_default);
  std::ostringstream sout;
  sout << file.rdbuf();

  return code_init.value() != 0 && code.value() == 0 && sout.str().find("[ERROR] Error mess") != std::string::npos;
}

TEST_CASE(TecslogInfoWarningErrorWithLowerDefaultLevelTest)
{
  tecslog::reset();

  std::string filename = "test.log";
  test::TestFileGuard guard(filename);

  std::error_code code_init = tecslog::init(filename, tecslog::Level::WARNING);
  std::error_code code = tecslog::error("Error mess");
  code = tecslog::info("First info mess");
  code = tecslog::setLevel(tecslog::Level::INFO);
  code = tecslog::info("Second info mess");
  code = tecslog::setLevel(tecslog::Level::ERROR);
  code = tecslog::warning("Warn mess");

  std::ifstream file(filename);
  std::ostringstream sout;
  sout << file.rdbuf();

  bool error_found = sout.str().find("[ERROR] Error mess") != std::string::npos;
  bool info_found = sout.str().find("[INFO] First info mess") != std::string::npos;
  bool info2_found = sout.str().find("[INFO] Second info mess") != std::string::npos;
  bool warn_found = sout.str().find("[WARNING] Warn mess") != std::string::npos;

  return code_init.value() == 0 && code.value() == 0 && error_found && !info_found && info2_found && !warn_found;
}

TEST_CASE(TecslogCorrectLogTest)
{
  tecslog::reset();

  std::string filename = "test.log";
  test::TestFileGuard guard(filename);

  std::error_code code_init = tecslog::init(filename, tecslog::Level::INFO);
  std::error_code code = tecslog::log(tecslog::Level::INFO, "Info mess");
  code = tecslog::log(tecslog::Level::WARNING, "Warn mess");
  code = tecslog::log(tecslog::Level::ERROR, "Error mess");

  std::ifstream file(filename);
  std::ostringstream sout;
  sout << file.rdbuf();

  bool info_found = sout.str().find("[INFO] Info mess") != std::string::npos;
  bool warn_found = sout.str().find("[WARNING] Warn mess") != std::string::npos;
  bool error_found = sout.str().find("[ERROR] Error mess") != std::string::npos;

  return code_init.value() == 0 && code.value() == 0 && info_found && warn_found && error_found;
}

TEST_CASE(TecslogLogWithCorrectStrLevelTest)
{
  tecslog::reset();

  std::string filename = "test.log";
  test::TestFileGuard guard(filename);

  std::error_code code_init = tecslog::init(filename, tecslog::Level::INFO);
  std::error_code code = tecslog::log("INFO", "Info mess");
  code = tecslog::log("WARNING", "Warn mess");
  code = tecslog::log("ERROR", "Error mess");

  std::ifstream file(filename);
  std::ostringstream sout;
  sout << file.rdbuf();

  bool info_found = sout.str().find("[INFO] Info mess") != std::string::npos;
  bool warn_found = sout.str().find("[WARNING] Warn mess") != std::string::npos;
  bool error_found = sout.str().find("[ERROR] Error mess") != std::string::npos;

  return code_init.value() == 0 && code.value() == 0 && info_found && warn_found && error_found;
}

TEST_CASE(TecslogLogWithInorrectStrLevelTest)
{
  tecslog::reset();

  std::string filename = "test.log";
  test::TestFileGuard guard(filename);

  std::error_code code_init = tecslog::init(filename, tecslog::Level::WARNING);
  std::error_code code = tecslog::log("BOOL", "Info mess");
  code = tecslog::log("INCORRECT", "Warn mess");

  std::ifstream file(filename);
  std::ostringstream sout;
  sout << file.rdbuf();

  return code_init.value() == 0 && code.value() != 0 && sout.str().empty();
}
