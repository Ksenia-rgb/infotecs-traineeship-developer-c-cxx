#include "test-runner.hpp"

#include <filesystem>

test::TestRegistrator::TestRegistrator(const std::string& name, std::function< bool() > func)
{
  getTests().push_back({name, func});
}

test::TestFileGuard::TestFileGuard(const std::string& filename):
  filename_(filename)
{
  std::filesystem::remove(filename_);
}

test::TestFileGuard::~TestFileGuard()
{
  std::filesystem::remove(filename_);
}

std::vector< test::TestCase >& test::getTests()
{
  static std::vector< TestCase > tests;
  return tests;
}

int test::runAllTests()
{
  int passed = 0, failed = 0;
  for (const TestCase& test: getTests())
  {
    std::cout << "[ RUN ] " << test.name << '\n';
    if (test.func())
    {
      std::cout << "[ OK ] " << test.name << '\n';
      passed++;
    }
    else
    {
      std::cout << "[ FAIL ] " << test.name << '\n';
      failed++;
    }
  }
  std::cout << "Summary: Passed " << passed << " Failed " << failed << '\n';
  return failed == 0 ? 0 : 1;
}
