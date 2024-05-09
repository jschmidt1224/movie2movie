
#ifndef __LINK_H__
#define __LINK_H__

#include "name.h"

class Link {
public:
  Link(actorId n, float w) : id(n), weight(w){};
  Link(actorId n) : Link(n, 1){};
  Link() : Link(""){};
  actorId id;
  float weight;
};

namespace boost {
namespace serialization {

template <class Archive>
void serialize(Archive &ar, Link &l, const unsigned int version = 0) {
  ar &l.id;
  ar &l.weight;
}

} // namespace serialization
} // namespace boost

#endif // __LINK_H__
