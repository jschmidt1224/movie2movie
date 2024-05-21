
#ifndef __MOVIE_H__
#define __MOVIE_H__

#include <boost/archive/text_iarchive.hpp>
#include <boost/archive/text_oarchive.hpp>
#include <boost/graph/adjacency_list.hpp>
#include <boost/graph/graph_selectors.hpp>
#include <boost/serialization/serialization.hpp>
#include <boost/serialization/set.hpp>
#include <boost/serialization/string.hpp>
#include <string>
#include <unordered_set>

class Link;

using namespace std;
using boost::adjacency_list, boost::vecS, boost::undirectedS;

typedef int movieId;
typedef adjacency_list<vecS, vecS, undirectedS, movieId, Link> graph;
typedef graph::vertex_descriptor gnode_descr;
typedef int actorId;

class Movie {
public:
  Movie(movieId _id, string _name, int _year, double _rating = 0, int _votes = 0)
      : id(_id), name(_name), year(_year), rating(_rating), votes(_votes), cast(), node(0), tmdb_refresh(0){};
  Movie() : Movie(0, "", 0){};
  movieId id;
  string name;
  int year;
  double rating;
  int votes;
  set<actorId> cast;
  gnode_descr node;
  time_t tmdb_refresh;

  friend std::ostream &operator<<(std::ostream &os, const Movie &m) {
    os << m.id << ",\"" << m.name << "\"," << m.year << "," << m.rating << "," << m.node << "," << m.tmdb_refresh
       << ",[";
    for (const actorId &a : m.cast) {
      os << a << ";";
    }
    os << "]";
    return os;
  }
};

#endif // __MOVIE_H__
