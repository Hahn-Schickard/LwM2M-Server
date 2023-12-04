#ifndef __LWM2M_MODEL_ELEMENT_ID_HPP
#define __LWM2M_MODEL_ELEMENT_ID_HPP

#include <cstdint>
#include <optional>
#include <string>
#include <vector>
#include <stdexcept>

namespace LwM2M {
/**
 * @addtogroup Device_Model
 * @{
 */

struct MissingOptionalID : public std::logic_error {
  MissingOptionalID(const std::string& full_id, const std::string id_type)
      : logic_error("ElementID " + full_id + " has no " + id_type + " ID") {}
};

/**
 * @brief Contains an identification number for a given Device Model element
 *
 */
struct ElementID {
  /**
   * @brief Create a default Object ID
   *
   * @note Used for testing only
   *
   */
  ElementID() = default;

  /**
   * @brief Create an Object ID
   *
   */
  ElementID(uint16_t object);

  /**
   * @brief Create an Object instance ID
   *
   */
  ElementID(uint16_t object, uint16_t object_instance);

  /**
   * @brief Create a Resource ID
   *
   */
  ElementID(uint16_t object, uint16_t object_instance, uint16_t resource);

  /**
   * @brief Create an Resource instance ID
   *
   */
  ElementID(uint16_t object, uint16_t object_instance, uint16_t resource,
      uint16_t resource_instance);

  /**
   * @brief Creates a subelement ID by using a parent ID values as it's base.
   *
   * For example calling @code {.cpp}
   * ElementID(ElementID(2),0)
   * @endcode
   * will create a new Element ID containing string ID 2/0
   *
   * @throws std::invalid_argument when all Element IDs are already assigned
   */
  ElementID(const ElementID& id, uint16_t sub_id);

  /**
   * @brief Get the Object ID
   *
   * @return uint16_t
   */
  uint16_t getObjectID() const;

  /**
   * @brief Get the Object Instance ID
   *
   * @throws std::logic_error when there is no value
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
   * @throws std::logic_error when there is no value
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
  bool hasResourceID() const;

  /**
   * @brief Get the Resource Instance ID
   *
   * @throws std::logic_error when there is no value
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
   * @brief Converts the entire Element ID into a string, each sub ID separated
   * via `/` character
   *
   * Example: Resource 2 of Object instance 1 of Object 0 would equate to string
   * "0/1/2" where:
   *  | | |
   *  | |  ⁻⁻ Resource ID
   *  |  ⁻⁻ Object Instance ID
   *   ⁻⁻ Object Id
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

  /**
   * @brief Returns hashed value of all internal sub-ids
   *
   * @return std::size_t -  up to 8 bytes long value
   */
  std::size_t hash() const;

  /**
   * @brief Returns the size of current Element ID
   *
   * - 2 Bytes for object ids
   * - 4 Bytes for object instance ids
   * - 6 Bytes for Resource ids
   * - 8 Bytes for Resource instance ids
   *
   * @return std::size_t
   */
  std::size_t size() const;

private:
  uint16_t object_;
  std::optional<uint16_t> object_instance_ = std::nullopt;
  std::optional<uint16_t> resource_ = std::nullopt;
  std::optional<uint16_t> resource_instance_ = std::nullopt;
};

/**
 * @brief Checks if given values are identical
 *
 * This means that each given sub id is identical
 *
 * @param lhs - left hand side value
 * @param rhs - right hand side value
 * @return true - if lhs and rhs are equal
 * @return false - if lhs and rhs are NOT equal
 */
bool operator==(const ElementID& lhs, const ElementID& rhs);

/**
 * @brief Checks if given values are not identical
 *
 * @param lhs - left hand side value
 * @param rhs - right hand side value
 * @return true - if lhs and rhs are NOT equal
 * @return false - if lhs and rhs are equal
 */
bool operator!=(const ElementID& lhs, const ElementID& rhs);

/**
 * @brief Checks if left hand value is less than right hand value
 *
 * Element ID uses weighted checks for each sub ID. This means that:
 * ElementID(1,2,3) < ElementID(2,0,0) is true
 * ElementID(1,2,3) < ElementID(1,3,0) is true
 * ElementID(1,2,3) < ElementID(1,2,4) is true
 * ElementID(1,2,3) < ElementID(1,0,5) is false
 *
 * @param lhs - left hand side value
 * @param rhs - right hand side value
 * @return true - if lhs is LESS than rhs
 * @return false - if lhs is MORE or EQUAL to rhs
 */
bool operator<(const ElementID& lhs, const ElementID& rhs);

/**
 * @brief Checks if left hand value is more than right hand value
 *
 * Element ID uses weighted checks for each sub ID. This means that:
 * ElementID(1,2,3) > ElementID(0,0,0) is true
 * ElementID(1,2,3) > ElementID(1,1,0) is true
 * ElementID(1,2,3) > ElementID(1,2,1) is true
 * ElementID(1,2,3) > ElementID(1,3,0) is false
 *
 * @param lhs - left hand side value
 * @param rhs - right hand side value
 * @return true - if lhs is MORE than rhs
 * @return false - if lhs is LESS or EQUAL to rhs
 */
bool operator>(const ElementID& lhs, const ElementID& rhs);

/**
 * @brief Checks if left hand value is less or equal than right hand value
 *
 * Element ID uses weighted checks for each sub ID. This means that:
 * ElementID(0,0,0) <= ElementID(0,0,0) is true
 * ElementID(1,2,3) <= ElementID(2,0,0) is true
 * ElementID(1,2,3) <= ElementID(1,3,0) is true
 * ElementID(1,2,3) <= ElementID(1,2,4) is true
 * ElementID(1,2,3) <= ElementID(1,0,5) is false
 *
 * @param lhs - left hand side value
 * @param rhs - right hand side value
 * @return true - if lhs is LESS or EQUAL to rhs
 * @return false - if lhs is MORE than rhs
 */
bool operator<=(const ElementID& lhs, const ElementID& rhs);

/**
 * @brief Checks if left hand value is more or equal than right hand value
 *
 * Element ID uses weighted checks for each sub ID. This means that:
 * ElementID(0,0,0) <= ElementID(0,0,0) is true
 * ElementID(1,2,3) >= ElementID(0,0,0) is true
 * ElementID(1,2,3) >= ElementID(1,1,0) is true
 * ElementID(1,2,3) >= ElementID(1,2,1) is true
 * ElementID(1,2,3) >= ElementID(1,3,0) is false
 *
 * @param lhs - left hand side value
 * @param rhs - right hand side value
 * @return true - if lhs is MORE or EQUAL to rhs
 * @return false - if lhs is LESS than rhs
 */
bool operator>=(const ElementID& lhs, const ElementID& rhs);

using ElementIDs = std::vector<ElementID>;

/** @}*/
} // namespace LwM2M

namespace std {
/**
 * @ingroup  Device_Model
 */
/**
 * @brief std::hash specialization for LwM2M::ElementID values
 *
 * @tparam  - LwM2M::ElementID
 */
template <> struct hash<LwM2M::ElementID> {
  std::size_t operator()(const LwM2M::ElementID& id) const;
};
} // namespace std

#endif //__LWM2M_MODEL_ELEMENT_ID_HPP