
#include <boost/graph/adjacency_list.hpp>
#include <boost/graph/dijkstra_shortest_paths.hpp>
#include <fstream>
#include <iostream>

#include "db.h"
#include "movie.h"
#include "name.h"

using boost::vertex_index, boost::predecessor_map;
using std::cout;

inline vector<string> split(string s) {
  stringstream ss(s);
  string tmp;
  vector<string> columns;
  while (getline(ss, tmp, '\t'))
    columns.push_back(tmp);
  return columns;
}

void Db::build_movies() {
  ifstream fTitles("imdb_data/title.basics.movie.tsv");
  string header, line;
  int count = 0;

  getline(fTitles, header);

  // Reading Titles file
  while (getline(fTitles, line)) {
    vector<string> columns = split(line);

    movieId id = columns[0];
    string name = columns[2];
    if (columns[3] != name) {
      name += "(" + columns[3] + ")";
    }
    movies.insert({id, Movie(id, name)});
    count++;
  }
  fTitles.close();
  cout << "Titles: " << count << endl;
}

void Db::build_principals() {
  ifstream fRoles("imdb_data/title.principals.roles.tsv");
  string header, line;
  int count = 0;

  getline(fRoles, header);

  while (getline(fRoles, line)) {
    vector<string> columns = split(line);

    movieId mid = columns[0];
    actorId aid = columns[2];
    string r = columns[3];
    // If movie id is not in movies
    if (movies.find(mid) != movies.end()) {
      // If actor id is not in names add it
      if (names.find(aid) == names.end())
        names[aid] = Name(aid);
      names[aid].movies.insert(mid);
      count++;
    }
  }
  fRoles.close();
  cout << "Roles: " << count << endl;
}

void Db::build_names() {
  ifstream fNames("imdb_data/name.basics.tsv");
  string header, line;
  int count = 0;

  getline(fNames, header);

  while (getline(fNames, line)) {
    vector<string> columns = split(line);

    actorId id = columns[0];
    string name = columns[1];
    if (names.find(id) != names.end()) {
      names[id].name = name;
      count++;
    }
  }
  fNames.close();
  cout << "Names: " << count << endl;
}

void Db::build_graph() {
  cout << "Building Graph" << endl;
  for (auto [k, v] : movies) {
    // cout << v.name << endl;
    movies[k].node = add_vertex(k, g);
  }
  int count = 0;
  for (auto it = names.begin(); it != names.end(); it++) {
    // cout << it->second.name << endl;
    for (auto it1 = it->second.movies.begin(); it1 != it->second.movies.end();
         it1++) {
      for (auto it2 = std::next(it1); it2 != it->second.movies.end(); it2++) {
        // std::cout << (*it).second.name << ": {" << (*it1) << ", " << (*it2)
        //           << "}" << std::endl;
        add_edge(movies[*it1].node, movies[*it2].node, Link(it->second.id, 1),
                 g);
        // add_edge(movies[*it2].node, movies[*it1].node, g);
        count++;
      }
    }
  }
  cout << "Edges: " << count << endl;
}

void Db::solve_graph(gnode_descr _start) {
  // cout << "Start node" << _start << " " << movies[g[_start]].name << endl;
  predecessors = vector<gnode_descr>(num_vertices(g));
  distances = vector<float>(num_vertices(g));
  auto weight_map = get(&Link::weight, g);
  start = _start;
  cout << "Starting solver\n";
  dijkstra_shortest_paths(
      g, start,
      predecessor_map(make_iterator_property_map(predecessors.begin(),
                                                 get(vertex_index, g)))
          .distance_map(make_iterator_property_map(distances.begin(),
                                                   get(vertex_index, g)))
          .weight_map(weight_map));
  // cout << "Done solving\n";
}

void Db::print_path(gnode_descr end) {
  // cout << "End node" << end << " " << movies[g[end]].name << endl;
  gnode_descr n = end;
  int i;
  for (i = 0; i < 25; i++) {
    cout << movies[g[n]].name;
    if (n == start)
      break;

    vector<actorId> actors = get_edges_between(n, predecessors[n]);
    cout << "<-(";
    for (auto a : actors) {
      cout << names[a].name << ",";
    }
    cout << ")<-";
    n = predecessors[n];
  }
  cout << endl << "Count: " << i;
}

gnode_descr Db::get_node(movieId mId) { return movies[mId].node; }

vector<actorId> Db::get_edges(gnode_descr a) {
  vector<actorId> r;
  graph_t::out_edge_iterator ei, ei_end;
  for (boost::tie(ei, ei_end) = boost::out_edges(a, g); ei != ei_end; ei++) {
    gnode_descr target = boost::target(*ei, g);
    gnode_descr source = boost::source(*ei, g);
    cout << movies[g[source]].name << "->" << names[g[*ei].id].name << "->"
         << movies[g[target]].name << endl;

    r.push_back(g[*ei].id);
  }
  return r;
}

vector<actorId> Db::get_edges_between(gnode_descr a, gnode_descr b) {
  vector<actorId> r;
  graph_t::out_edge_iterator ei, ei_end;
  for (boost::tie(ei, ei_end) = boost::out_edges(a, g); ei != ei_end; ei++) {
    gnode_descr target = boost::target(*ei, g);
    if (target == b) {
      // cout << names[g[*ei].id].name << endl;
      r.push_back(g[*ei].id);
    }
  }
  return r;
}
