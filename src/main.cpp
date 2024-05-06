
#include <boost/graph/adjacency_list.hpp>
#include <fstream>
#include <iostream>
#include <list>
#include <regex>
#include <sstream>
#include <string>
#include <unordered_map>

#include "db.h"
#include "movie.h"

using namespace std;

inline vector<string> split(string s) {
  stringstream ss(s);
  string tmp;
  vector<string> columns;
  while (getline(ss, tmp, '\t'))
    columns.push_back(tmp);
  return columns;
}

int main() {
  Db db;
  db.build_movies();
  db.build_principals();
  db.build_names();
  db.build_graph();
  movieId start = "tt2717822";
  movieId end = "tt13833688";
  cout << "Start " << db.movies[start].name << endl;
  cout << "End " << db.movies[end].name << endl;

  db.solve_graph(db.get_node(start));
  db.print_path(db.get_node(end));
  return 0;
}
