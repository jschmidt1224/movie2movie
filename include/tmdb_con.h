
#ifndef __TMDB_CON_H__
#define __TMDB_CON_H__

#include <atomic>
#include <boost/json.hpp>
#include <boost/json/parser.hpp>
#include <boost/json/value.hpp>
#include <curl/curl.h>
#include <mutex>
#include <string>

using std::string, std::mutex, std::atomic_int, std::atomic_bool;

typedef string movieId;

class TMDBConnection {
public:
  TMDBConnection();
  ~TMDBConnection();
  int get(string url, FILE *f = stdout);
  int get(string url, string filename);
  int get_popmovies(int page = 1);
  int get_imdb(movieId m);
  int get_cast(int id);
  int get_person();
  void count_func();
  boost::json::value parse_file(string fname);
  static string base_url;

private:
  CURL *hnd;
  mutex hnd_lock;
  curl_slist *headers;
  boost::json::parser p;
};

#endif // __TMDB_CON_H__
