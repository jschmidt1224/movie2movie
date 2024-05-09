
#include <boost/graph/adjacency_list.hpp>
#include <boost/serialization/library_version_type.hpp>
#include <fstream>
#include <functional>
#include <iostream>
#include <list>
#include <regex>
#include <sstream>
#include <string>
#include <unordered_map>

#include "console.h"
#include "db.h"
#include "movie.h"

inline vector<string> split(string s) {
  stringstream ss(s);
  string tmp;
  vector<string> columns;
  while (getline(ss, tmp, '\t'))
    columns.push_back(tmp);
  return columns;
}

int main(int argc, char **argv) {
  Db db;
  CommandConsole console;
  console.insert("build_db", std::bind(&Db::build_db, &db));
  console.insert("build_graph", std::bind(&Db::build_graph, &db));
  console.insert("save_db", std::bind(&Db::save_db, &db));
  console.insert("load_db", std::bind(&Db::load_db, &db));
  console.insert("save_graph", std::bind(&Db::save_graph, &db));
  console.insert("load_graph", std::bind(&Db::load_graph, &db));
  console.insert("load", std::bind(&Db::load, &db));
  console.insert("search", std::bind(&Db::search, &db, std::placeholders::_1));
  console.insert("show_results", std::bind(&Db::show_results, &db));
  console.insert("filter", std::bind(&Db::filter, &db, std::placeholders::_1));
  console.start();
  return 0;
  db.build_movies();
  db.build_principals();
  db.build_names();
  if (argc >= 3) {
    movieId start = argv[1]; // "tt27304026";
    movieId end = argv[2];   // "tt0106489";
    cout << "Start " << db.movies[start].name << endl;
    cout << "End " << db.movies[end].name << endl;
    std::ofstream test("test.txt");
    boost::archive::text_oarchive t(test);
    t << db.movies[start];
    db.start = db.get_node(start);
    db.build_graph();
    db.solve_graph();
    db.print_path(db.get_node(end));
  }
  return 0;
}
