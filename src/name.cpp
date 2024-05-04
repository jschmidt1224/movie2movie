
#include "name.h"

Name::Name(actorId _id, string _name) : id(_id), name(_name) {}

Name::Name(actorId _id) : Name(_id, "") {}

Name::Name() : Name("tt0", "No One") {}
