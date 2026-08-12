#include <tecslog/tecslog.hpp>

#include "test-runner.hpp"

TEST_CASE(TecslogСorrectInitTest)
{
  tecslog::reset();

  std::string filename = "test.log";
  test::TestFileGuard guard(filename);

  std::error_code code = tecslog::init(filename, tecslog::Level::WARNING);
  bool file = tecslog::getLogFile() == filename;
  bool level = tecslog::getDefaultLevel() == tecslog::Level::WARNING;

  return code.value() == 0 && file && level;
}

TEST_CASE(TecslogCorrectInitWithStrLevelTest)
{
  tecslog::reset();

  std::string filename = "test.log";
  test::TestFileGuard guard(filename);

  std::error_code code = tecslog::init(filename, "INFO");
  bool file = tecslog::getLogFile() == filename;
  bool level = tecslog::getDefaultLevel() == tecslog::Level::INFO;

  return code.value() == 0 && file && level;
}

TEST_CASE(TecslogIncorrectInitWithStrLevelTest)
{
  tecslog::reset();

  std::string filename = "test.log";
  test::TestFileGuard guard(filename);

  std::error_code code = tecslog::init(filename, "INCORRECT");
  bool file = tecslog::getLogFile() == filename;
  bool level = tecslog::getDefaultLevel() == tecslog::Level::INFO;

  return code.value() != 0 && file && level;
}

TEST_CASE(TecslogIncorrectInitWithNonExistedFileTest)
{
  std::error_code code = tecslog::init("logs/unreal-directory/test.log", tecslog::Level::ERROR);
  return code.value() != 0;
}
