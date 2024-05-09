
#include <boost/algorithm/string.hpp>
#include <boost/archive/text_iarchive.hpp>
#include <boost/graph/adjacency_list.hpp>
#include <boost/graph/dijkstra_shortest_paths.hpp>
#include <boost/serialization/library_version_type.hpp>
#include <boost/serialization/unordered_map.hpp>
#include <boost/serialization/unordered_set.hpp>
#include <fstream>
#include <iostream>
#include <stdexcept>

#include "db.h"
#include "movie.h"
#include "name.h"

using boost::vertex_index, boost::predecessor_map;
using std::cout;

void Db::build_movies() {
  ifstream fTitles("imdb_data/title.basics.movie.tsv");
  string header, line;
  int count = 0;

  getline(fTitles, header);

  // Reading Titles file
  while (getline(fTitles, line)) {
    vector<string> columns;
    boost::split(columns, line, boost::is_any_of("\t"));

    movieId id = columns[0];
    string name = columns[2];
    int year = 0;
    cout << columns[5] << endl;
    if (columns[5] != "\\N")
      year = stoi(columns[5]);
    if (columns[3] != name) {
      name += "(" + columns[3] + ")";
    }
    movies.insert({id, Movie(id, name, year)});
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
    vector<string> columns;
    boost::split(columns, line, boost::is_any_of("\t"));

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
    vector<string> columns;
    boost::split(columns, line, boost::is_any_of("\t"));

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

void Db::build_db() {
  build_movies();
  build_principals();
  build_names();
}

void Db::save_db() {
  ofstream of("movie_cast.db");
  boost::archive::text_oarchive oa(of);
  oa << movies;
  oa << names;
  of.close();
}

void Db::load_db() {
  ifstream inf("movie_cast.db");
  boost::archive::text_iarchive ia(inf);
  ia >> movies;
  ia >> names;
  inf.close();
}

void Db::save_graph() {
  ofstream of("movie_cast.graph");
  boost::archive::text_oarchive oa(of);
  oa << movies;
  oa << names;
  of.close();
}

void Db::load_graph() {
  ifstream inf("movie_cast.graph");
  boost::archive::text_iarchive ia(inf);
  ia >> movies;
  ia >> names;
  inf.close();
}

void Db::load() {
  load_db();
  load_graph();
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

void Db::solve_graph() {
  // cout << "Start node" << _start << " " << movies[g[_start]].name << endl;
  predecessors = vector<gnode_descr>(num_vertices(g));
  distances = vector<float>(num_vertices(g));
  auto weight_map = get(&Link::weight, g);
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

void Db::show_results() {
  int i = 0;
  for (auto &mId : results) {
    Movie &m = movies[mId];
    cout << i << "\t" << m.id << "\t" << m.name << "\t" << m.year << endl;
    i++;
  }
}

void Db::search(vector<string> args) {
  string name_filt = "";
  if (args.size() > 0) {
    name_filt = args[0];
  }
  results.clear();
  for (auto &m : movies) {
    if (boost::icontains(m.second.name, name_filt)) {
      results.push_back(m.second.id);
    }
  }
  show_results();
}

void Db::filter(vector<string> args) {
  string name_filt = "";
  if (args.size() > 0) {
    name_filt = args[0];
  }
  vector<movieId>::iterator it = results.begin();
  while (it != results.end()) {
    if (boost::icontains(movies[*it].name, name_filt)) {
      it++;
    } else {
      it = results.erase(it);
    }
  }
}

void Db::set_start(vector<string> args) {
  if (args.size() > 0) {
    try {
      if (boost::starts_with(args[0], "tt")) {
        start = movies.at(args[0]).node;
      } else {

        unsigned long index = stoul(args[0]);
        if (index < results.size()) {
          start = movies.at(results[index]).node;
        } else {
          cout << "Out of results range (" << results.size() << ")\n";
        }
      }
    } catch (std::invalid_argument const &e) {
      cout << "Not an integer\n";
    } catch (std::out_of_range const &e) {
      cout << "Out of range\n";
    }
  }
}
