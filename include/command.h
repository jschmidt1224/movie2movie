
#ifndef __COMMAND_H__
#define __COMMAND_H__

class Command {
public:
  Command(){};
  virtual void execute() = 0;
};

#endif
