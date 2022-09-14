
#include "Observer.hpp"
#include "DataFormat.hpp"

#include "HSCUL/String.hpp"
#include "Variant_Visitor.hpp"

#include <iomanip>
#include <iostream>

using namespace std;

namespace LwM2M_Example {
Observer::Observer(ObservablePtr element) : ObserverInterface(element) {}

void handleData(const DataFormatPtr& data, DataType data_type) {
  switch (data_type) {
  case DataType::STRING: {
    cout << data->get<string>();
    break;
  }
  case DataType::SIGNED_INTEGER: {
    cout << data->get<int64_t>();
    break;
  }
  case DataType::UNSIGNED_INTEGER: {
    cout << data->get<uint64_t>();
    break;
  }
  case DataType::FLOAT: {
    cout << data->get<double>();
    break;
  }
  case DataType::BOOLEAN: {
    cout << std::boolalpha << data->get<bool>();
    break;
  }
  case DataType::OPAQUE: {
    cout << HSCUL::hexify(data->get<std::vector<uint8_t>>());
    break;
  }
  case DataType::TIME: {
    cout << data->get<TimeStamp>().toString();
    break;
  }
  case DataType::OBJECT_LINK: {
    cout << data->get<ObjectLink>().toString();
    break;
  }
  case DataType::CORE_LINK: {
    cout << "CoRE Link format is not supported";
    break;
  }
  case DataType::NONE: {
    cout << "void";
    break;
  }
  default: {
    cout << "Could not resolve data type of given data element!";
    break;
  }
  }
}

void handleTargetContent(
    TargetContent data, Observable::ObservedDataTypes observed_data_typese) {
  cout << data.first.toString() << " ";
  auto data_type = observed_data_typese.find(data.first);
  if (data_type != observed_data_typese.end()) {
    handleData(data.second, data_type->second);
  } else {
    cout << " has no observed data value";
  }
}

void handleTargetContentVector(TargetContentVector vector,
    const Observable::ObservedDataTypes& observed_data_types) {
  cout << "{";
  for (auto target_data = vector.begin(); target_data != vector.end();
       ++target_data) {
    handleTargetContent(*target_data, observed_data_types);
    if (next(target_data) != vector.end()) {
      cout << ",";
    }
  }
  cout << "}";
}

void handleElementID(ElementID id) { cout << id.toString(); }

void handleElementIDs(ElementIDs ids) {
  cout << "{";
  for (auto id = ids.begin(); id != ids.end(); ++id) {
    handleElementID(*id);
    if (next(id) != ids.end()) {
      cout << ",";
    }
  }
  cout << "}";
}

void handleTargetAttribute(TargetAttribute attribute) {
  handleElementID(attribute.first);
  cout << " " << attribute.second->toString();
}

void handleTargetAttributes(TargetAttributes attributes) {
  cout << "{";
  for (auto attribute = attributes.begin(); attribute != attributes.end();
       ++attribute) {
    handleTargetAttribute(*attribute);
    if (next(attribute) != attributes.end()) {
      cout << ",";
    }
  }
  cout << "}";
}

void Observer::handleEvent(PayloadDataPtr payload) {
  if (payload) {
    cout << "Received: ";
    match(*payload,
        [&](DataFormatPtr data) {
          handleData(data, getObservedDataTypes().begin()->second);
          cout << endl;
        },
        [&](TargetContent data) {
          handleTargetContent(data, getObservedDataTypes());
          cout << endl;
        },
        [&](TargetContentVector data) {
          handleTargetContentVector(data, getObservedDataTypes());
          cout << endl;
        },
        [&](ElementID id) {
          handleElementID(id);
          cout << endl;
        },
        [&](ElementIDs ids) {
          handleElementIDs(ids);
          cout << endl;
        },
        [&](TargetAttributes attributes) {
          handleTargetAttributes(attributes);
          cout << endl;
        });
  } else {
    string error = " received a notification with an empty data payload";
  }
}
} // namespace LwM2M_Example