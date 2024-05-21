
#include <boost/json.hpp>
#include <boost/json/value.hpp>
#include <cstdio>
#include <fstream>
#include <functional>
#include <iostream>
#include <sstream>
#include <string>
#include <thread>

#include "tmdb_con.h"

using std::ifstream, std::string, std::stringstream;

string TMDBConnection::base_url = string("https://api.themoviedb.org/3/");

TMDBConnection::TMDBConnection() {
  hnd_lock.lock();
  hnd = curl_easy_init();
  headers = NULL;
  ifstream inf("tmdb/read.token");
  string auth;
  inf >> auth;
  inf.close();
  auth = "Authorization: Bearer " + auth;
  headers = curl_slist_append(headers, "accept: application/json");
  headers = curl_slist_append(headers, auth.c_str());
  curl_easy_setopt(hnd, CURLOPT_HTTPHEADER, headers);
  hnd_lock.unlock();
}

TMDBConnection::~TMDBConnection() {}

int TMDBConnection::get(string url, FILE *f) {
  hnd_lock.lock();
  curl_easy_setopt(hnd, CURLOPT_CUSTOMREQUEST, "GET");
  curl_easy_setopt(hnd, CURLOPT_WRITEDATA, f);
  curl_easy_setopt(hnd, CURLOPT_URL, url.c_str());

  CURLcode ret = curl_easy_perform(hnd);
  hnd_lock.unlock();
  return ret;
}

int TMDBConnection::get(string url, string filename) {
  FILE *fp = fopen(filename.c_str(), "wb");
  if (fp == NULL)
    return -1;
  int ret = get(url, fp);
  fclose(fp);
  return ret;
}

int TMDBConnection::get_popmovies(int page, int year) {
  string discover = TMDBConnection::base_url;
  discover += "discover/movie?include_adult=false&include_video=false";
  discover += "&primary_release_year=" + std::to_string(year) + "&language=en-US&page=" + std::to_string(page);
  discover += "&sort_by=vote_count.desc";
  return get(discover, POP_MOVIES);
}

int TMDBConnection::get_cast(movieId id) {
  string cast = TMDBConnection::base_url;
  cast += "movie/" + std::to_string(id) + "/credits?language=en-US";
  return get(cast, CAST);
}

int TMDBConnection::get_person(int id) {
  string person = TMDBConnection::base_url;
  person += "person/" + std::to_string(id) + "?language=en-US";
  return get(person, PERSON);
}

boost::json::value TMDBConnection::parse_file(string fname) {
  ifstream ifs(fname);
  stringstream buf;
  buf << ifs.rdbuf();
  return boost::json::parse(buf.str());
}
