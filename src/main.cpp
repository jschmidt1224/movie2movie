
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
#include "tmdb_con.h"

int main(int argc, char **argv) {
  Db db;
  CommandConsole console("movie2movie$ ");
  console.insert("build_db", std::bind(&Db::build_db, &db));
  console.insert("build_graph", std::bind(&Db::build_graph, &db));
  console.insert("save_db", std::bind(&Db::save_db, &db));
  console.insert("load_db", std::bind(&Db::load_db, &db));
  console.insert("save_graph", std::bind(&Db::save_graph, &db));
  console.insert("load_graph", std::bind(&Db::load_graph, &db));
  console.insert("load", std::bind(&Db::load, &db));
  console.insert("save", std::bind(&Db::save, &db));
  console.insert("search", std::bind(&Db::search, &db, std::placeholders::_1));
  console.insert("show", std::bind(&Db::show_results, &db));
  console.insert("solve", std::bind(&Db::solve, &db));
  console.insert("filter", std::bind(&Db::filter, &db, std::placeholders::_1));
  console.insert("start",
                 std::bind(&Db::set_start, &db, std::placeholders::_1));
  console.insert("end", std::bind(&Db::set_end, &db, std::placeholders::_1));
  TMDBConnection tmdb;
  // tmdb.get(TMDBConnection::base_url +
  //          "discover/"
  //          "movie?include_adult=false&include_video=false&language=en-US&page="
  //          "1&sort_by=vote_count.desc");
  console.start();
  return 0;
}

// TODOs
// 1 Redo save maps to csv
// 2 Redo load maps to use rapidcsv
// 3 Add date touched to movies and actors
// 4 Add sort by date touched
// 5 Add commands to start update threads
// 6 Add update thread to loop through movies and get movie details, cast and
// cast details
//
