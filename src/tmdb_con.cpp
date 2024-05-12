
#include <fstream>
#include <iostream>
#include <string>

#include "tmdb_con.h"

using std::ifstream, std::string;

string TMDBConnection::base_url = string("https://api.themoviedb.org/3/");

TMDBConnection::TMDBConnection() {
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
}

int TMDBConnection::get(string url, FILE *f) {
  curl_easy_setopt(hnd, CURLOPT_CUSTOMREQUEST, "GET");
  curl_easy_setopt(hnd, CURLOPT_WRITEDATA, f);
  curl_easy_setopt(hnd, CURLOPT_URL, url.c_str());

  CURLcode ret = curl_easy_perform(hnd);
  return ret;
}
