#include "HaSLL/LoggerManager.hpp"
#include "LwM2M_Server/Server.hpp"

#include <iostream>

using namespace std;
using namespace HaSLL;
using namespace LwM2M;

int main() {
  try {
    LoggerManager::initialise(makeDefaultRepository());
    try {

      auto server = make_unique<Server>("config/serverConfig.json");

      for (const auto& binding : server->getRegisterBindingNames()) {
        cout << binding << endl;
      }

    } catch (const exception& ex) {
      cerr << ex.what() << endl;
      LoggerManager::terminate();
      exit(EXIT_FAILURE);
    }
    LoggerManager::terminate();
  } catch (const exception& ex) {
    cerr << ex.what() << endl;
    exit(EXIT_FAILURE);
  }

  cout << "Integration test successful" << endl;
  exit(EXIT_SUCCESS);
}
