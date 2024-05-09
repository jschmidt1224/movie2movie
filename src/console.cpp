
#include <cstring>
#include <functional>
#include <iomanip>
#include <iostream>
#include <sstream>
#include <string>
#include <vector>

#include "console.h"

using std::cout, std::cin, std::endl, std::pair;
using std::vector, std::stringstream, std::bind;

inline vector<string> split(const string s) {
  stringstream ss(s);
  string tmp;
  vector<string> args;
  while (ss >> std::quoted(tmp)) {
    args.push_back(tmp);
  }
  return args;
}

static void echo(vector<string> args) {
  for (auto &s : args) {
    cout << s << endl;
  }
}

CommandConsole::CommandConsole(string pre) {
  conPrefix = pre;
  comMap.insert(pair{"echo", &echo});
  function<void(vector<string>)> e =
      bind(&CommandConsole::exit, this, std::placeholders::_1);
  comMap.insert(pair{"exit", e});
  comMap.insert(pair{"quit", e});
  running = false;
}

CommandConsole::CommandConsole() : CommandConsole("Shell>") {}

CommandConsole::~CommandConsole() {}

void CommandConsole::run(string text) {
  vector<string> args = split(text);
  if (args.size() > 0) {
    string command = args[0];
    args.erase(args.begin());
    for (auto &[cmd, callback] : comMap) {
      if (cmd.compare(command) == 0) {
        callback(args);
        return;
      }
    }
    cout << "Command not found: " << command << endl;
  }
}

void CommandConsole::start() {
  string in;
  running = true;
  while (running) {
    cout << conPrefix;
    getline(cin, in);
    run(in);
  }
}

void CommandConsole::insert(string s, function<void(vector<string>)> f) {
  comMap.insert(pair{s, f});
}

void CommandConsole::exit(vector<string>) {
  cout << "Exitting\n";
  running = false;
}
