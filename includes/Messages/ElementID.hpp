#ifndef __LWM2M_MODEL_ELEMENT_ID_HPP
#define __LWM2M_MODEL_ELEMENT_ID_HPP

#include <cstdint>
#include <string>
#include <variant>
#include <vector>

namespace LwM2M {

struct ResourceID {
  ResourceID(uint16_t id, uint16_t instance);
  ResourceID(uint16_t id,
             std::vector<uint16_t> instances = std::vector<uint16_t>{});

  ResourceID() = default;
  ResourceID(const ResourceID &mE) = default;
  ResourceID(ResourceID &&mE) = default;
  ResourceID &operator=(const ResourceID &mE) = default;
  ResourceID &operator=(ResourceID &&mE) = default;

  uint16_t getID() const;
  std::vector<uint16_t> getResourceInstanceIDs() const;
  std::string toString() const;
  std::vector<std::string> toStrings() const;
  std::size_t size() const;
  bool hasInstances() const;

  friend bool operator==(const ResourceID &lhs, const ResourceID &rhs);

private:
  uint16_t id_;
  std::vector<uint16_t> resource_instances_;
};

struct ObjectInstanceID {
  using ResourceIDs = std::vector<ResourceID>;

  ObjectInstanceID(uint16_t id, ResourceID resource);
  ObjectInstanceID(uint16_t id, ResourceIDs resources = ResourceIDs());

  ObjectInstanceID() = default;
  ObjectInstanceID(const ObjectInstanceID &mE) = default;
  ObjectInstanceID(ObjectInstanceID &&mE) = default;
  ObjectInstanceID &operator=(const ObjectInstanceID &mE) = default;
  ObjectInstanceID &operator=(ObjectInstanceID &&mE) = default;

  uint16_t getID() const;
  ResourceIDs getResourceIDs() const;
  std::string toString() const;
  std::vector<std::string> toStrings() const;
  std::size_t size() const;
  bool hasResources() const;

  friend bool operator==(const ObjectInstanceID &lhs,
                         const ObjectInstanceID &rhs);

private:
  uint16_t id_;
  ResourceIDs resources_;
};

struct ObjectID {
  using ObjectInstanceIDs = std::vector<ObjectInstanceID>;

  ObjectID(uint16_t id, ObjectInstanceID instance);
  ObjectID(uint16_t id, ObjectInstanceIDs instances = ObjectInstanceIDs());
  ObjectID(uint16_t id, uint16_t instance, uint16_t resource);
  ObjectID(uint16_t id, uint16_t instance, uint16_t resource,
           uint16_t resource_instance);
  ObjectID(uint16_t id, uint16_t instance, uint16_t resource,
           std::vector<uint16_t> resource_instances);

  ObjectID() = default;
  ObjectID(const ObjectID &mE) = default;
  ObjectID(ObjectID &&mE) = default;
  ObjectID &operator=(const ObjectID &mE) = default;
  ObjectID &operator=(ObjectID &&mE) = default;

  uint16_t getID() const;
  ObjectInstanceIDs getObjectInstanceIDs() const;
  std::string toString() const;
  std::vector<std::string> toStrings() const;
  std::size_t size() const;

  friend bool operator==(const ObjectID &lhs, const ObjectID &rhs);

private:
  uint16_t id_;
  ObjectInstanceIDs instances_;
};

using ObjectIDs = std::vector<ObjectID>;
} // namespace LwM2M

namespace std {
template <> struct hash<LwM2M::ObjectID> {
  /**
   * @brief Calculates the hash of a given LwM2M::ObjectID. Calculation
   * is based only on the internal id and NOT on the entire list of
   * ObjectInstanceIDs.
   *
   * @param object
   * @return std::size_t
   */
  std::size_t operator()(const LwM2M::ObjectID &object) const;
};

template <> struct hash<LwM2M::ObjectInstanceID> {
  /**
   * @brief Calculates the hash of a given LwM2M::ObjectInstanceID. Calculation
   * is based only on the internal id and NOT on the entire list of ResourceIDs.
   *
   * @param instance
   * @return std::size_t
   */
  std::size_t operator()(const LwM2M::ObjectInstanceID &instance) const;
};

template <> struct hash<LwM2M::ResourceID> {
  /**
   * @brief Calculates the hash of a given LwM2M::ResourceID. Calculation is
   * based only on the internal id and NOT on the entire list of
   * ResourceInstanceIDs.
   *
   * @param resource
   * @return std::size_t
   */
  std::size_t operator()(const LwM2M::ResourceID &resource) const;
};
} // namespace std

#endif //__LWM2M_MODEL_ELEMENT_ID_HPP