
#ifndef __NAME_H__
#define __NAME_H__

#include <boost/serialization/boost_unordered_set.hpp>
#include <string>
#include <unordered_set>

#include "movie.h"

typedef int actorId;

class Name {
public:
  Name(actorId _id, string _name) : id(_id), name(_name){};
  Name(actorId _id) : Name(_id, ""){};
  Name() : Name(0){};
  actorId id;
  string name;
  unordered_set<movieId> movies;

  friend std::ostream &operator<<(std::ostream &os, const Name &n) {
    os << n.id << ",\"" << n.name << "\",[";
    for (auto m : n.movies) {
      os << m << ";";
    }
    os << "]";
    return os;
  }
};

#endif // __NAME_H__
