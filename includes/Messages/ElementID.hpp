#ifndef __LWM2M_MODEL_ELEMENT_ID_HPP
#define __LWM2M_MODEL_ELEMENT_ID_HPP

#include <cstdint>
#include <optional>
#include <string>
#include <vector>

namespace LwM2M {

struct ElementID {
  ElementID(uint16_t object);
  ElementID(uint16_t object, uint16_t object_instance);
  ElementID(uint16_t object, uint16_t object_instance, uint16_t resource);
  ElementID(uint16_t object, uint16_t object_instance, uint16_t resource,
      uint16_t resource_instance);

  uint16_t getObjectID() const;
  /**
   * @brief Get the Object Instance ID
   *
   * @throws std::bad_optional_access when there is no value
   *
   * @return uint16_t
   */
  uint16_t getObjectInstanceID() const;
  /**
   * @brief Checks if Object Instance ID is available
   *
   * @return true
   * @return false
   */
  bool hasObjectInstanceID() const;
  /**
   * @brief Get the Resource ID
   *
   * @throws std::bad_optional_access when there is no value
   *
   * @return uint16_t
   */
  uint16_t getResourceID() const;
  /**
   * @brief Checks if Resource ID is available
   *
   * @return true
   * @return false
   */
  bool hastResourceID() const;
  /**
   * @brief Get the Resource Instance ID
   *
   * @throws std::bad_optional_access when there is no value
   *
   * @return uint16_t
   */
  uint16_t getResourceInstanceID() const;
  /**
   * @brief Checks if Resource Instance ID is available
   *
   * @return true
   * @return false
   */
  bool hasResourceInstanceID() const;
  /**
   * @brief Converts the entire Element ID into a string, each sub ID seperated
   * via `/` character
   *
   * @return std::string
   */
  std::string toString() const;
  /**
   * @brief Converts each sub ID within Element ID into a vector of strings,
   * starting with the object then the object instance, then resource and
   * resource instance
   *
   * @return std::vector<std::string>
   */
  std::vector<std::string> toStrings() const;
  std::size_t hash() const;
  std::size_t size() const;

private:
  uint16_t object_;
  std::optional<uint16_t> object_instance_ = std::nullopt;
  std::optional<uint16_t> resource_ = std::nullopt;
  std::optional<uint16_t> resource_instance_ = std::nullopt;
};

bool operator==(const ElementID& lhs, const ElementID& rhs);
bool operator!=(const ElementID& lhs, const ElementID& rhs);
bool operator<(const ElementID& lhs, const ElementID& rhs);
bool operator>(const ElementID& lhs, const ElementID& rhs);
bool operator<=(const ElementID& lhs, const ElementID& rhs);
bool operator>=(const ElementID& lhs, const ElementID& rhs);

using ElementIDs = std::vector<ElementID>;
} // namespace LwM2M

namespace std {
template <> struct hash<LwM2M::ElementID> {
  std::size_t operator()(const LwM2M::ElementID& id) const;
};
} // namespace std

#endif //__LWM2M_MODEL_ELEMENT_ID_HPP