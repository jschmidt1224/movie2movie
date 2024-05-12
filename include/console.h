
#ifndef __CONSOLE_H__
#define __CONSOLE_H__

#include <functional>
#include <map>
#include <string>
#include <vector>

using std::string, std::map, std::function, std::vector;

class CommandConsole {
public:
  CommandConsole(string pre);
  CommandConsole();
  ~CommandConsole();
  void start();
  void run(string text);
  void exit(vector<string>);
  void insert(string, function<void(vector<string>)>);
  void set_cleanup(function<void(void)>);
  map<string, function<void(vector<string>)>> comMap;

private:
  string conPrefix;
  bool running;
  function<void(void)> cleanup;
};

#endif
