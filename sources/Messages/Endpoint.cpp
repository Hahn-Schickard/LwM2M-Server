#include "Endpoint.hpp"

using namespace std;
using namespace LwM2M;

Endpoint::Endpoint(string sms_number) : sms_number_(sms_number) {}
Endpoint::Endpoint(string endpoint_address, unsigned int endpoint_port)
    : endpoint_address_(endpoint_address), endpoint_port_(endpoint_port) {}

string Endpoint::toString() {
  if (!endpoint_address_.empty()) {
    return endpoint_address_ + ":" + to_string(endpoint_port_);
  } else {
    return sms_number_;
  }
}
