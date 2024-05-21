
#ifndef __TMDB_CON_H__
#define __TMDB_CON_H__

#include <atomic>
#include <boost/json.hpp>
#include <boost/json/parser.hpp>
#include <boost/json/value.hpp>
#include <curl/curl.h>
#include <mutex>
#include <string>

#define POP_MOVIES "tmp/popmovies.csv"
#define CAST "tmp/cast.csv"
#define PERSON "tmp/person.csv"

using std::string, std::mutex, std::atomic_int, std::atomic_bool;

typedef int movieId;

class TMDBConnection {
public:
  TMDBConnection();
  ~TMDBConnection();
  int get(string url, FILE *f = stdout);
  int get(string url, string filename);
  int get_popmovies(int page = 1, int year = 2024);
  int get_cast(movieId id);
  int get_person(movieId id);
  void count_func();
  boost::json::value parse_file(string fname);
  static string base_url;

private:
  CURL *hnd;
  mutex hnd_lock;
  curl_slist *headers;
};

#endif // __TMDB_CON_H__
