#include <sstream>

#include <tecslog/tecslog.hpp>

#include "test-runner.hpp"

TEST_CASE(TecslogCorrectSetLevelTest)
{
  tecslog::reset();

  std::string filename = "test.log";
  test::TestFileGuard guard(filename);

  tecslog::init(filename, tecslog::Level::WARNING);
  std::error_code code = tecslog::setLevel(tecslog::Level::INFO);
  bool level = tecslog::getDefaultLevel() == tecslog::Level::INFO;

  return code.value() == 0 && level;
}

TEST_CASE(TecslogCorrctSetSameLevelTest)
{
  tecslog::reset();

  std::string filename = "test.log";
  test::TestFileGuard guard(filename);

  tecslog::init(filename, "ERROR");
  std::error_code code = tecslog::setLevel(tecslog::Level::ERROR);
  bool level = tecslog::getDefaultLevel() == tecslog::Level::ERROR;

  return code.value() == 0 && level;
}

TEST_CASE(TecslogCorrectSetStrLevelTest)
{
  tecslog::reset();

  std::string filename = "test.log";
  test::TestFileGuard guard(filename);

  tecslog::init(filename, "WARNING");
  std::error_code code = tecslog::setLevel("INFO");
  bool level = tecslog::getDefaultLevel() == tecslog::Level::INFO;

  return code.value() == 0 && level;
}

TEST_CASE(TecslogIncorrectSetStrLevelTest)
{
  tecslog::reset();

  std::string filename = "test.log";
  test::TestFileGuard guard(filename);

  tecslog::init(filename, "WARNING");
  std::error_code code = tecslog::setLevel("INCORRECT");
  bool level = tecslog::getDefaultLevel() == tecslog::Level::WARNING;

  return code.value() != 0 && level;
}

TEST_CASE(TecslogPrintPossibleLevelsTest)
{
  tecslog::reset();

  std::string filename = "test.log";
  test::TestFileGuard guard(filename);

  tecslog::init(filename, tecslog::Level::WARNING);
  
  std::stringstream sout;
  tecslog::printPossibleLevels(sout);
  return sout.str() == "0 INFO\n1 WARNING\n2 ERROR";
}

TEST_CASE(TecslogCorrectParseLevelTest)
{
  std::optional< tecslog::Level > level = tecslog::parseLevel("ERROR");
  return level.has_value() && level.value() == tecslog::Level::ERROR;
}

TEST_CASE(TecslogIncorrectParseLevelTest)
{
  std::optional< tecslog::Level > level = tecslog::parseLevel("INCORRECT");
  return !level.has_value();
}


