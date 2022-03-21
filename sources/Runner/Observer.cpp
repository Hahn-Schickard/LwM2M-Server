
#include "Observer.hpp"
#include "DataFormat.hpp"
#include "Variant_Visitor.hpp"

#include <iostream>

using namespace std;

namespace LwM2M_Example {
Observer::Observer(ObservablePtr element) : ObserverInterface(element) {}

void handleData(DataFormatPtr data) {}

void handleTargetContent(TargetContent data) {
  cout << data.first.toString() << " ";
  handleData(data.second);
}

void handleTargetContentVector(TargetContentVector vector) {
  cout << "{";
  for (auto target_data = vector.begin(); target_data != vector.end();
       ++target_data) {
    handleTargetContent(*target_data);
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
    match(*payload,
          [&](DataFormatPtr data) {
            handleData(data);
            cout << endl;
          },
          [&](TargetContent data) {
            handleTargetContent(data);
            cout << endl;
          },
          [&](TargetContentVector data) {
            handleTargetContentVector(data);
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