
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

#include <boost/json/src.hpp>

#include "console.h"
#include "db.h"
#include "movie.h"
#include "tmdb_con.h"

int main(int argc, char **argv) {
  Db db;
  CommandConsole console("movie2movie$ ");
  db.register_commands(console);
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
