
#include <boost/algorithm/string.hpp>
#include <boost/algorithm/string/predicate.hpp>
#include <boost/archive/text_iarchive.hpp>
#include <boost/graph/adj_list_serialize.hpp>
#include <boost/graph/adjacency_list.hpp>
#include <boost/graph/dijkstra_shortest_paths.hpp>
#include <boost/json.hpp>
#include <boost/json/value.hpp>
#include <boost/range/algorithm/replace_if.hpp>
#include <boost/serialization/library_version_type.hpp>
#include <boost/serialization/unordered_map.hpp>
#include <boost/serialization/unordered_set.hpp>
#include <fstream>
#include <iostream>
#include <stdexcept>

#include "rapidcsv/rapidcsv.h"

#include "console.h"
#include "db.h"
#include "movie.h"
#include "name.h"

using boost::vertex_index, boost::predecessor_map;
using boost::json::value;
using std::cout;

vector<int> split_csvlist(string l) {
  vector<string> s;
  vector<int> r;
  cout << l << endl;
  boost::split(s, l, boost::is_any_of(";[]"));
  for (auto &e : s) {
    if (!e.empty())
      r.push_back(stoi(e));
  }
  return r;
}

Db::~Db() {
  movies.clear();
  names.clear();
  g.m_edges.clear();
  g.m_vertices.clear();
  g.clear();
}

void Db::save_db() {
  int nMovies = 0, nNames = 0;
  cout << "Saving..." << endl;
  ofstream of(MOVIES);
  of << "id,name,year,rating,node,tmdb_update,cast" << endl;
  map_mutex.lock();
  for (auto &m : movies) {
    of << m.second << endl;
    nMovies++;
  }
  map_mutex.unlock();
  of.close();
  of.open(NAMES);
  of << "id,name,movies" << endl;
  map_mutex.lock();
  for (auto &[k, a] : names) {
    of << a << endl;
    nNames++;
  }
  map_mutex.unlock();
  of.close();
  cout << "Movies: " << nMovies << ", Names: " << nNames << endl;
  // ofstream of("movie_cast.db");
  // boost::archive::text_oarchive oa(of);
  // oa << movies;
  // oa << names;
  // of.close();
}

// void Db::load_db() {
//   ifstream inf("movie_cast.db");
//   boost::archive::text_iarchive ia(inf);
//   ia >> movies;
//   ia >> names;
//   inf.close();
// }

void Db::load_db() {
  int count = 0;
  rapidcsv::Document d(MOVIES);
  cout << "Reading db" << endl;
  for (unsigned long i = 0; i < d.GetRowCount(); i++) {
    movieId id = d.GetCell<movieId>("id", i);
    cout << id << " " << endl;
    string name = d.GetCell<string>("name", i);
    int year = d.GetCell<int>("year", i);
    cout << year << endl;
    float rating = d.GetCell<float>("rating", i);
    string sActors = d.GetCell<string>("cast", i);
    unsigned long node = d.GetCell<unsigned long>("node", i);
    vector<actorId> lActors = split_csvlist(sActors);
    cout << count << " " << name << endl;
    map_mutex.lock();
    movies[id] = Movie(id, name, year, rating);
    movies[id].cast.insert(lActors.begin(), lActors.end());
    movies[id].node = node;
    map_mutex.unlock();
    count++;
  }
  d.Clear();
  cout << "Titles: " << count << endl;
  d.Load(NAMES);
  count = 0;
  for (unsigned long i = 0; i < d.GetRowCount(); i++) {
    actorId id = d.GetCell<actorId>("id", i);
    string name = d.GetCell<string>("name", i);
    string sMovies = d.GetCell<string>("movies", i);
    vector<movieId> lMovies = split_csvlist(sMovies);
    cout << count << " " << name << endl;
    map_mutex.lock();
    names[id] = Name(id, name);
    names[id].movies.insert(lMovies.begin(), lMovies.end());
    map_mutex.unlock();
    count++;
  }
  d.Clear();
  cout << "Names: " << count << endl;
}

void Db::save_graph() {
  ofstream of("movie_cast.graph");
  boost::archive::text_oarchive oa(of);
  oa << g;
  of.close();
}

void Db::load_graph() {
  ifstream inf("movie_cast.graph");
  boost::archive::text_iarchive ia(inf);
  ia >> g;
  inf.close();
}

void Db::load() {
  load_db();
  load_graph();
}

void Db::save() {
  save_db();
  save_graph();
}

void Db::build_graph() {
  cout << "Building Graph" << endl;
  g.m_edges.clear();
  g.m_vertices.clear();
  g.clear();
  for (auto [k, v] : movies) {
    // cout << v.name << endl;
    movies[k].node = add_vertex(k, g);
  }
  int count = 0;
  for (auto it = names.begin(); it != names.end(); it++) {
    // cout << it->second.name << endl;
    for (auto it1 = it->second.movies.begin(); it1 != it->second.movies.end(); it1++) {
      for (auto it2 = std::next(it1); it2 != it->second.movies.end(); it2++) {
        // std::cout << (*it).second.name << ": {" << (*it1) << ", " << (*it2)
        //           << "}" << std::endl;
        add_edge(movies[*it1].node, movies[*it2].node, Link(it->second.id, 1), g);
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
  dijkstra_shortest_paths(g, start,
                          predecessor_map(make_iterator_property_map(predecessors.begin(), get(vertex_index, g)))
                              .distance_map(make_iterator_property_map(distances.begin(), get(vertex_index, g)))
                              .weight_map(weight_map));
  // cout << "Done solving\n";
}

void Db::solve() {
  if (start != 0 || end != 0) {
    solve_graph();
    print_path(end);
  }
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
    cout << "->(";
    for (auto a : actors) {
      cout << names[a].name << ",";
    }
    cout << ")->";
    n = predecessors[n];
  }
  cout << endl << "Count: " << i << endl;
}

gnode_descr Db::get_node(movieId mId) { return movies[mId].node; }

vector<actorId> Db::get_edges(gnode_descr a) {
  vector<actorId> r;
  graph_t::out_edge_iterator ei, ei_end;
  for (boost::tie(ei, ei_end) = boost::out_edges(a, g); ei != ei_end; ei++) {
    gnode_descr target = boost::target(*ei, g);
    gnode_descr source = boost::source(*ei, g);
    cout << movies[g[source]].name << "->" << names[g[*ei].id].name << "->" << movies[g[target]].name << endl;

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
  if (end < num_vertices(g)) {
    cout << "Current Start: " << g[end] << ", " << movies[g[end]].name << endl;
  }
  if (start < num_vertices(g)) {
    cout << "Current End:   " << g[start] << ", " << movies[g[start]].name << endl;
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
  show_results();
}

void Db::set_start(vector<string> args) {
  if (args.size() > 0) {
    try {
      if (boost::starts_with(args[0], ".")) {
        end = movies.at(atoi(args[0].c_str() + 1)).node;
      } else {
        unsigned long index = stoul(args[0]);
        if (index < results.size()) {
          end = movies.at(results[index]).node;
        } else {
          cout << "Out of results range (" << results.size() << ")\n";
        }
      }
    } catch (std::invalid_argument const &e) {
      cout << args[0] << " not an integer\n";
    } catch (std::out_of_range const &e) {
      cout << args[0] << " not found\n";
    }
  }
}

void Db::set_end(vector<string> args) {
  if (args.size() > 0) {
    try {
      if (boost::starts_with(args[0], ".")) {
        start = movies.at(atoi(args[0].c_str() + 1)).node;
      } else {

        unsigned long index = stoul(args[0]);
        if (index < results.size()) {
          start = movies.at(results[index]).node;
        } else {
          cout << "Out of results range (" << results.size() << ")\n";
        }
      }
    } catch (std::invalid_argument const &e) {
      cout << args[0] << "not an integer\n";
    } catch (std::out_of_range const &e) {
      cout << args[0] << " not found\n";
    }
  }
}

void Db::update_titles() {
  int count = 0;
  int total_pages = 10;
  int year = 2024;
  for (; year >= 1900; year--) {
    cout << "Year: " << year << endl;
    for (int current_page = 1; current_page <= total_pages; current_page++) {
      cout << "Getting Page: " << current_page << endl;
      tmdb.get_popmovies(current_page, year);
      value v = tmdb.parse_file(POP_MOVIES);
      if (v.as_object().contains("success")) {
        if (v.at("success").as_bool() == false) {
          cout << v << endl;
          return;
        }
      }
      v = v.at("results");
      for (auto &m : v.as_array()) {
        movieId id = m.at("id").as_int64();
        string name = m.at("title").as_string().c_str();
        string date = m.at("release_date").as_string().c_str();
        int votes = m.at("vote_count").as_int64();
        double rating = m.at("vote_average").as_double();
        vector<string> tmp;
        boost::split(tmp, date, boost::is_any_of("-"));
        int year = stoi(tmp[0]);
        // cout << name << endl;
        map_mutex.lock();
        if (movies.find(id) == movies.end()) {
          movies[id] = Movie(id, name, year, rating, votes);
          count++;
        } else {
          movies[id].name = name;
          movies[id].year = year;
          movies[id].rating = rating;
          movies[id].votes = votes;
        }
        movies[id].tmdb_refresh = std::time(0);
        map_mutex.unlock();
        update_credits(id);
      }
    }
    save_db();
  }
}

void Db::update_credits(movieId movie) {
  tmdb.get_cast(movie);
  value v = tmdb.parse_file(CAST);
  for (auto &a : v.at("cast").as_array()) {
    actorId id = a.at("id").as_int64();
    string name = a.at("name").as_string().c_str();
    // cout << id << ": " << name << endl;
    map_mutex.lock();
    if (names.find(id) == names.end()) {
      names[id] = Name(id, name);
    }
    names[id].movies.insert(movie);
    movies[movie].cast.insert(id);
    map_mutex.unlock();
  }
}

void Db::register_commands(CommandConsole &console) {
  console.insert("build_graph", std::bind(&Db::build_graph, this));
  console.insert("save_db", std::bind(&Db::save_db, this));
  console.insert("load_db", std::bind(&Db::load_db, this));
  console.insert("save_graph", std::bind(&Db::save_graph, this));
  console.insert("load_graph", std::bind(&Db::load_graph, this));
  console.insert("load", std::bind(&Db::load, this));
  console.insert("save", std::bind(&Db::save, this));
  console.insert("update_titles", std::bind(&Db::update_titles, this));
  console.insert("search", std::bind(&Db::search, this, std::placeholders::_1));
  console.insert("show", std::bind(&Db::show_results, this));
  console.insert("solve", std::bind(&Db::solve, this));
  console.insert("filter", std::bind(&Db::filter, this, std::placeholders::_1));
  console.insert("start", std::bind(&Db::set_start, this, std::placeholders::_1));
  console.insert("end", std::bind(&Db::set_end, this, std::placeholders::_1));
}
