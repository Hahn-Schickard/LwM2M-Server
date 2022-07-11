#include "ModelType.hpp"

using namespace std;

namespace LwM2M {
string toString(LwM2M_Version version) {
  switch (version) {
  case LwM2M_Version::V1_0: {
    return "1.0";
  }
  case LwM2M_Version::V1_1: {
    return "1.1.";
  }
  default: {
    return "Unrecognized";
  }
  }
}

string toString(BindingType binding) {
  switch (binding) {
  case BindingType::UDP: {
    return "UDP";
  }
  case BindingType::TCP: {
    return "TCP";
  }
  case BindingType::SMS: {
    return "SMS";
  }
  case BindingType::NON_IP: {
    return "Non IP";
  }
  default: {
    return "Malformated";
  }
  }
}
} // namespace LwM2M
