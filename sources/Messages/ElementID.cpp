#include "ElementID.hpp"
#include "Variant_Visitor.hpp"

using namespace std;

namespace LwM2M {
ResourceID::ResourceID(uint16_t id, uint16_t instance)
    : id_(id), resource_instances_(vector<uint16_t>{instance}) {}

ResourceID::ResourceID(uint16_t id, std::vector<uint16_t> instances)
    : id_(id), resource_instances_(instances) {}

uint16_t ResourceID::getID() const { return id_; }

std::vector<uint16_t> ResourceID::getResourceInstanceIDs() const {
  return resource_instances_;
}

string ResourceID::toString() const {
  string result;
  auto instances = toStrings();
  if (instances.size() > 1) {
    result = "{";
    for (auto instance : instances) {
      result += instance;
    }
    result += "}";
  } else if (!instances.empty()) {
    result = instances[0];
  } else {
    result = to_string(id_);
  }

  return result;
}

vector<string> ResourceID::toStrings() const {
  vector<string> result;
  for (auto instance_id : resource_instances_) {
    result.emplace_back(to_string(id_) + "/" + to_string(instance_id));
  }
  return result;
}

size_t ResourceID::size() const {
  return resource_instances_.size() + sizeof(id_);
}

bool ResourceID::hasInstances() const { return resource_instances_.empty(); }

bool operator==(const ResourceID &lhs, const ResourceID &rhs) {
  return (lhs.id_ == rhs.id_ &&
          lhs.resource_instances_ == rhs.resource_instances_)
             ? true
             : false;
}

ObjectInstanceID::ObjectInstanceID(uint16_t id, ResourceID resource)
    : id_(id), resources_(ResourceIDs{resource}) {}

ObjectInstanceID::ObjectInstanceID(uint16_t id, ResourceIDs resources)
    : id_(id), resources_(resources) {}

uint16_t ObjectInstanceID::getID() const { return id_; }

ObjectInstanceID::ResourceIDs ObjectInstanceID::getResourceIDs() const {
  return resources_;
}

string ObjectInstanceID::toString() const {
  string result;
  auto resources = toStrings();
  if (resources.size() > 1) {
    result = "{";
    for (auto instance : resources) {
      result += instance;
    }
    result += "}";
  } else if (!resources.empty()) {
    result = resources[0];
  } else {
    result = to_string(id_);
  }

  return result;
}

vector<string> ObjectInstanceID::toStrings() const {
  vector<string> result;
  for (auto resource : resources_) {
    if (resource.hasInstances()) {
      for (auto resource_instance : resource.toStrings()) {
        result.emplace_back(to_string(id_) + "/" + resource_instance);
      }
    } else {
      result.emplace_back(to_string(id_) + "/" + resource.toString());
    }
  }
  return result;
}

size_t ObjectInstanceID::size() const {
  return resources_.size() + sizeof(id_);
}

bool ObjectInstanceID::hasResources() const { return resources_.empty(); }

bool operator==(const ObjectInstanceID &lhs, const ObjectInstanceID &rhs) {
  return (lhs.id_ == rhs.id_ && lhs.resources_ == rhs.resources_) ? true
                                                                  : false;
}

ObjectID::ObjectID(uint16_t id, uint16_t instance, uint16_t resource)
    : ObjectID(id, ObjectInstanceID(instance, ResourceID(resource))) {}

ObjectID::ObjectID(uint16_t id, uint16_t instance, uint16_t resource,
                   uint16_t resource_instance)
    : ObjectID(id, ObjectInstanceID(instance,
                                    ResourceID(resource, resource_instance))) {}

ObjectID::ObjectID(uint16_t id, uint16_t instance, uint16_t resource,
                   vector<uint16_t> resource_instances)
    : ObjectID(id, ObjectInstanceID(instance,
                                    ResourceID(resource, resource_instances))) {
}

ObjectID::ObjectID(uint16_t id, ObjectInstanceID instance)
    : id_(id), instances_(ObjectInstanceIDs{instance}) {}

ObjectID::ObjectID(uint16_t id, ObjectInstanceIDs instances)
    : id_(id), instances_(instances) {}

uint16_t ObjectID::getID() const { return id_; }

ObjectID::ObjectInstanceIDs ObjectID::getObjectInstanceIDs() const {
  return instances_;
}

string ObjectID::toString() const {
  string result;
  auto instances = toStrings();
  if (instances.size() > 1) {
    result = "{";
    for (auto instance : instances) {
      result += instance;
    }
    result += "}";
  } else if (!instances.empty()) {
    result = instances[0];
  } else {
    result = to_string(id_);
  }

  return result;
}

vector<string> ObjectID::toStrings() const {
  vector<string> result;
  for (auto instance : instances_) {
    if (instance.hasResources()) {
      for (auto resource : instance.toStrings()) {
        result.emplace_back(to_string(id_) + "/" + resource);
      }
    } else {
      result.emplace_back(to_string(id_) + "/" + instance.toString());
    }
  }
  return result;
}

size_t ObjectID::size() const { return instances_.size() + sizeof(id_); }

bool operator==(const ObjectID &lhs, const ObjectID &rhs) {
  return (lhs.id_ == rhs.id_ && lhs.instances_ == rhs.instances_) ? true
                                                                  : false;
}
} // namespace LwM2M

namespace std {

std::size_t hash<LwM2M::ResourceID>::
operator()(const LwM2M::ResourceID &resource) const {
  return resource.getID();
}

std::size_t hash<LwM2M::ObjectInstanceID>::
operator()(const LwM2M::ObjectInstanceID &instance) const {
  return instance.getID();
}

std::size_t hash<LwM2M::ObjectID>::
operator()(const LwM2M::ObjectID &object) const {
  return object.getID();
}

} // namespace std
