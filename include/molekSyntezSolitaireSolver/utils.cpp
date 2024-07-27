#include "utils.hpp"

std::vector<std::string> split(const std::string& s, const std::string& delimiter) {
  if(delimiter.empty()) {
    std::vector<std::string> res;
    for(char c : s) {
      res.emplace_back(1, c);
    }
    return res;
  }
  size_t pos_start = 0, pos_end, delim_len = delimiter.length();
  std::string token;
  std::vector<std::string> res;

  while ((pos_end = s.find(delimiter, pos_start)) != std::string::npos) {
    token = s.substr (pos_start, pos_end - pos_start);
    pos_start = pos_end + delim_len;
    res.push_back (token);
  }

  res.push_back (s.substr (pos_start));
  return res;
}