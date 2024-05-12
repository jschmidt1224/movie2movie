
#ifndef __DB_H__
#define __DB_H__

#include <mutex>
#include <thread>
#include <unordered_map>
#include <vector>

#include "link.h"
#include "movie.h"
#include "name.h"

typedef adjacency_list<vecS, vecS, undirectedS, movieId, Link> graph_t;
typedef graph_t::vertex_descriptor gnode_descr;

using std::thread, std::mutex;

class Db {
public:
  Db()
      : movies(), names(), g(), predecessors(), distances(), start(0), end(0),
        results(), map_mutex(){};
  ~Db();
  unordered_map<movieId, Movie> movies;
  unordered_map<actorId, Name> names;
  graph_t g;
  vector<gnode_descr> predecessors;
  vector<float> distances;
  gnode_descr start;
  gnode_descr end;
  vector<movieId> results;
  thread update_existing;
  thread update_popular;
  mutex map_mutex;
  void build_movies();
  void build_principals();
  void build_names();
  void build_db();
  void save_db();
  void load_db();
  void build_graph();
  void save_graph();
  void load_graph();
  void load();
  void save();
  void solve_graph();
  void print_path(gnode_descr end);
  void solve();
  void write_movies();
  void write_names();
  void read_movies();
  void read_names();
  void show_results();
  void search(vector<string> args);
  void filter(vector<string> args);
  void filter_year(vector<string> args);
  void set_start(vector<string> args);
  void set_end(vector<string> args);
  gnode_descr get_node(movieId mId);
  vector<actorId> get_edges(gnode_descr a);
  vector<actorId> get_edges_between(gnode_descr a, gnode_descr b);
};

#endif // __DB_H__
