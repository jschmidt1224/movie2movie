
#ifndef __MOVIE_H__
#define __MOVIE_H__

#include <boost/archive/text_iarchive.hpp>
#include <boost/archive/text_oarchive.hpp>
#include <boost/graph/adjacency_list.hpp>
#include <boost/graph/graph_selectors.hpp>
#include <boost/serialization/serialization.hpp>
#include <boost/serialization/string.hpp>
#include <string>
#include <unordered_set>

class Link;

using namespace std;
using boost::adjacency_list, boost::vecS, boost::undirectedS;

typedef string movieId;
typedef adjacency_list<vecS, vecS, undirectedS, movieId, Link> graph;
typedef graph::vertex_descriptor gnode_descr;

class Movie {
public:
  Movie(movieId _id, string _name, int _year)
      : id(_id), name(_name), year(_year), rating(1), node(0){};
  Movie() : Movie("", "", 0){};
  movieId id;
  string name;
  int year;
  float rating;
  gnode_descr node;

  friend std::ostream &operator<<(std::ostream &os, const Movie &m) {
    os << m.id << ",\"" << m.name << "\"," << m.year << "," << m.rating;
    return os;
  }
};

namespace boost {
namespace serialization {

template <class Archive>
void serialize(Archive &ar, Movie &m, const unsigned int version = 0) {
  ar &m.id;
  ar &m.name;
  ar &m.year;
  ar &m.rating;
}

} // namespace serialization
} // namespace boost

#endif // __MOVIE_H__
