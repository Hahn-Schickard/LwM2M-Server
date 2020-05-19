#include "CoAP_Option.hpp"

#include <stdexcept>

using namespace std;
namespace CoAP {
CoAP_Option::CoAP_Option()
    : CoAP_Option(OptionNumber::RESERVED, 0, false, false, false, 0) {}

CoAP_Option::CoAP_Option(OptionNumber option_number, size_t option_size,
                         bool critical, bool repeatable, bool unsafe,
                         size_t max_size)
    : option_number_(option_number), option_size_(option_size),
      critical_(critical), repeatable_(repeatable), unsafe_(unsafe),
      max_size_(max_size) {
  if (max_size_ > option_size_) {
    string error_msg = "Given option exceeds maximum lenght value of " +
                       to_string(max_size_) + " bytes";
    throw out_of_range(error_msg);
  }
}

OptionNumber CoAP_Option::getOptionNumber() { return option_number_; }
size_t CoAP_Option::size() { return option_size_; }

bool CoAP_Option::isCritical() { return critical_; }
bool CoAP_Option::isRepeatable() { return repeatable_; }
bool CoAP_Option::isUnsafe() { return unsafe_; }
} // namespace CoAP