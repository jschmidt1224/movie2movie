
#ifndef __NAME_H__
#define __NAME_H__

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

#endif // __NAME_H__
