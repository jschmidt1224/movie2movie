
#ifndef __LINK_H__
#define __LINK_H__

#include "name.h"

class Link {
public:
  Link(actorId n, float w) : id(n), weight(w){};
  Link(actorId n) : Link(n, 0){};
  Link() : Link("", 0){};
  actorId id;
  float weight;
};

#endif // __LINK_H__
