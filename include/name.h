
#ifndef __NAME_H__
#define __NAME_H__

#include <boost/serialization/boost_unordered_set.hpp>
#include <string>
#include <unordered_set>

#include "movie.h"

typedef string actorId;

class Name {
public:
  Name(actorId _id, string _name) : id(_id), name(_name){};
  Name(actorId _id) : Name(_id, ""){};
  Name() : Name("tt0", ""){};
  actorId id;
  string name;
  unordered_set<movieId> movies;
};

namespace boost {
namespace serialization {

template <class Archive>
void serialize(Archive &ar, Name &n, const unsigned int version = 0) {
  ar &n.id;
  ar &n.name;
  ar &n.movies;
}

} // namespace serialization
} // namespace boost

#endif // __NAME_H__
