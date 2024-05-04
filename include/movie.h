
#ifndef __MOVIE_H__
#define __MOVIE_H__

#include <string>
#include <unordered_set>

using namespace std;

typedef string movieId;

class Movie {
public:
  Movie(movieId _id, string _name);
  movieId id;
  string name;
  float rating;
};

#endif
