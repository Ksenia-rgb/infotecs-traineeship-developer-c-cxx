#include <iomanip>
#include <iostream>
#include <functional>
#include <system_error>
#include <thread>
#include <unordered_map>

#include <tecslog/tecslog.hpp>

#include "io-commands.hpp"
#include "log-info.hpp"
#include "thread-safe-queue.hpp"

namespace
{
  void logFromQueue(demo::ThreadSafeQueue< demo::LogInfo >& queue, std::error_code& code)
  {
    while (!queue.isForceStopped())
    {
      demo::LogInfo* log_info = queue.front();
      if (log_info)
      {
        code = tecslog::log(log_info->level, log_info->message);
        if (code)
        {
          queue.forceStop();
        }
        queue.pop();
      }
    }
  }
}

int main(int argc, char** argv)
{
  using namespace std::placeholders;

  if (argc != 3)
  {
    std::cerr << "Incorrect programm argument count\n";
    return 1;
  }
  if (tecslog::init(argv[1], argv[2]))
  {
    std::cerr << "Incorrect argument for lib initialization\n";
    return 1;
  }

  demo::ThreadSafeQueue< demo::LogInfo > queue;
  std::error_code logger_code;
  std::thread logger(logFromQueue, std::ref(queue), std::ref(logger_code));

  demo::LogInfo info;
  std::unordered_map< std::string, std::function< std::error_code(std::istream&, std::ostream&) > > command_map;
  command_map["silence"] = demo::silenceCommand;
  command_map["default"] = demo::defaultCommand;
  command_map["log"] = std::bind(demo::logCommand, _1, _2, std::ref(info), std::ref(queue));
  command_map["help"] = demo::helpCommand;

  std::istream& in = std::cin;
  std::ostream& out = std::cout;

  std::string command;
  while (in >> command && !queue.isForceStopped())
  {
    auto command_iter = command_map.find(command);
    if (command_iter != command_map.end())
    {
      std::error_code code = command_iter->second(in, out);
      if (code)
      {
        std::cerr << code.message();
      }
      out << '\n';
    }
    else
    {
      std::cerr << "Invalid command\n";
      std::cin.clear();
      std::cin.ignore(std::numeric_limits< std::streamsize >::max(), '\n');
    }
  }
  if (queue.isForceStopped())
  {
    std::cerr << "Log error: " << logger_code.message() << '\n';
  }
  else
  {
    queue.forceStop();
  }
  logger.join();
}
