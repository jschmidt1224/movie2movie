
#ifndef __TMDB_CON_H__
#define __TMDB_CON_H__

#include <curl/curl.h>
#include <string>

using std::string;

class TMDBConnection {
public:
  TMDBConnection();
  int get(string url, FILE *f = stdout);
  int get_movies();
  int get_cast();
  int get_person();
  static string base_url;

private:
  CURL *hnd;
  curl_slist *headers;
};

#endif // __TMDB_CON_H__
