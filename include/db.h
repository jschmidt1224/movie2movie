
#ifndef __DB_H__
#define __DB_H__

#include <unordered_map>
#include <vector>

#include "link.h"
#include "movie.h"
#include "name.h"

typedef adjacency_list<vecS, vecS, undirectedS, movieId, Link> graph_t;
typedef graph_t::vertex_descriptor gnode_descr;

class Db {
public:
  Db() : movies(), names(), g(){};
  unordered_map<movieId, Movie> movies;
  unordered_map<actorId, Name> names;
  graph_t g;
  vector<gnode_descr> predecessors;
  vector<float> distances;
  gnode_descr start;
  void build_movies();
  void build_principals();
  void build_names();
  void build_graph();
  void solve_graph(gnode_descr start);
  void print_path(gnode_descr end);
  void write_movies();
  void write_names();
  void read_movies();
  void read_names();
  gnode_descr get_node(movieId mId);
  vector<actorId> get_edges(gnode_descr a);
  vector<actorId> get_edges_between(gnode_descr a, gnode_descr b);
};

#endif // __DB_H__
