
#ifndef __MOVIE_H__
#define __MOVIE_H__

#include <boost/graph/adjacency_list.hpp>
#include <boost/graph/graph_selectors.hpp>
#include <string>
#include <unordered_set>

class Link;

using namespace std;
using boost::adjacency_list, boost::vecS, boost::bidirectionalS;

typedef string movieId;
typedef adjacency_list<vecS, vecS, bidirectionalS, movieId, Link> graph;
typedef graph::vertex_descriptor gnode_descr;

class Movie {
public:
  Movie(movieId _id, string _name) : id(_id), name(_name), rating(1), node(0){};
  Movie() : Movie("", ""){};
  movieId id;
  string name;
  float rating;
  gnode_descr node;
};

#endif
