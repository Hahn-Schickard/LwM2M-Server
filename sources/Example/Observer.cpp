
#include "Observer.hpp"
#include "DataFormat.hpp"

#include "HSCUL/String.hpp"
#include "Variant_Visitor.hpp"

#include <iomanip>
#include <iostream>

using namespace std;

namespace LwM2M_Example {
Observer::Observer(const ObservablePtr& element) : ObserverInterface(element) {}

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

void handleTargetContent(const TargetContent& data,
    const Observable::ObservedDataTypes& observed_data_typese) {
  cout << data.first.toString() << " ";
  auto data_type = observed_data_typese.find(data.first);
  if (data_type != observed_data_typese.end()) {
    handleData(data.second, data_type->second);
  } else {
    cout << " has no observed data value";
  }
}

void handleTargetContentVector(const TargetContentVector& vector,
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

void handleElementID(const ElementID& id) { cout << id.toString(); }

void handleElementIDs(const ElementIDs& ids) {
  cout << "{";
  for (auto id = ids.begin(); id != ids.end(); ++id) {
    handleElementID(*id);
    if (next(id) != ids.end()) {
      cout << ",";
    }
  }
  cout << "}";
}

void handleTargetAttribute(const TargetAttribute& attribute) {
  handleElementID(attribute.first);
  cout << " " << attribute.second->toString();
}

void handleTargetAttributes(const TargetAttributes& attributes) {
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
  cout << "Received: ";
  if (payload) {
    match(
        *payload,
        [&](const DataFormatPtr& data) {
          handleData(data, getObservedDataTypes().begin()->second);
          cout << endl;
        },
        [&](const TargetContent& data) {
          handleTargetContent(data, getObservedDataTypes());
          cout << endl;
        },
        [&](const TargetContentVector& data) {
          handleTargetContentVector(data, getObservedDataTypes());
          cout << endl;
        },
        [&](const ElementID& id) {
          handleElementID(id);
          cout << endl;
        },
        [&](const ElementIDs& ids) {
          handleElementIDs(ids);
          cout << endl;
        },
        [&](const TargetAttributes& attributes) {
          handleTargetAttributes(attributes);
          cout << endl;
        });
  } else {
    cout << " a notification with an empty data payload" << endl;
  }
}
} // namespace LwM2M_Example