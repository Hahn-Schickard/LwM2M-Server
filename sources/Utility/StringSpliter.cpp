#include "StringSpliter.hpp"

#include <sstream>
#include <stdexcept>

using namespace std;

namespace utility {
vector<string> split(const string &s, char delimiter) {
  vector<string> tokens;
  string token;
  istringstream tokenStream(s);
  while (getline(tokenStream, token, delimiter)) {
    tokens.push_back(token);
  }
  return tokens;
}

vector<uint8_t> convert(const string &s) {
  vector<uint8_t> result;
  for (auto iterator = s.cbegin(); iterator != s.cend(); iterator++) {
    result.push_back(*iterator);
  }
  return result;
}
} // namespace utility