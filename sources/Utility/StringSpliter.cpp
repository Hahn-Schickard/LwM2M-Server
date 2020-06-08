#include "StringSpliter.hpp"

#include <sstream>
#include <stdexcept>

using namespace std;

namespace CoAP {
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
} // namespace utility
} // namespace CoAP