
#include <boost/graph/adjacency_list.hpp>
#include <fstream>
#include <iostream>
#include <list>
#include <regex>
#include <sstream>
#include <string>
#include <unordered_map>

#include "link.h"
#include "movie.h"
#include "name.h"

using namespace std;

using boost::adjacency_list, boost::vecS;
using boost::bidirectionalS, boost::add_vertex, boost::add_edge;

typedef adjacency_list<vecS, vecS, bidirectionalS, movieId, Link> graph;
typedef graph::vertex_descriptor gnode_descr;

inline vector<string> split(string s) {
  stringstream ss(s);
  string tmp;
  vector<string> columns;
  while (getline(ss, tmp, '\t'))
    columns.push_back(tmp);
  return columns;
}

int main() {
  cout << "Hello IMDb\n";

  ifstream fTitles("imdb_data/title.basics.movie.tsv");
  ifstream fNames("imdb_data/name.basics.tsv");
  ifstream fRoles("imdb_data/title.principals.roles.tsv");
  string line;
  int count = 0;

  if (!fTitles.is_open() || !fNames.is_open() || !fRoles.is_open()) {
    cout << "Unable to open file!\n";
  }

  unordered_map<movieId, Movie> movies;
  unordered_map<actorId, Name> names;
  string header;
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
  count = 0;

  // Read Principals
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
  count = 0;

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

  graph g;

  for (auto [k, v] : movies) {
    // cout << k << " " << v.name << endl;
    v.node = add_vertex(k, g);
  }

  for (auto it = names.begin(); it != names.end(); it++) {
    // cout << it->second.name << endl;
    for (auto it1 = it->second.movies.begin(); it1 != it->second.movies.end();
         it1++) {
      for (auto it2 = std::next(it1); it2 != it->second.movies.end(); it2++) {
        // std::cout << (*it).second.name << ": {" << (*it1) << ", " << (*it2)
        //           << "}" << std::endl;
        add_edge(movies[*it1].node, movies[*it2].node, g);
      }
    }
  }

  return 0;
}
