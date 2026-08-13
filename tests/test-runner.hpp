#include <iostream>
#include <vector>
#include <string>
#include <functional>

namespace test
{
  struct TestCase
  {
    std::string name;
    std::function< bool() > func;
  };

  class TestRegistrator
  {
  public:
    TestRegistrator(const std::string& name, std::function< bool() > func);
  };

  class TestFileGuard
  {
  public:
    TestFileGuard(const std::string& filename);
    ~TestFileGuard();
  private:
    std::string filename_;
  };

  std::vector< TestCase >& getTests();
  int runAllTests();

  #define TEST_CASE(name) \
    bool test_case_##name(); \
    static test::TestRegistrator reg_##name(#name, test_case_##name); \
    bool test_case_##name()
}
