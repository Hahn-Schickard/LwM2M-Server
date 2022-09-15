#ifndef __LWM2M_OBJECT_LINK_DATA_TYPE_HPP
#define __LWM2M_OBJECT_LINK_DATA_TYPE_HPP

#include <cstdint>
#include <string>

namespace LwM2M {
/**
 * @brief ObjectLink - indicates a link to another object
 *
 */
struct ObjectLink {
  const uint16_t object_id_;
  const uint16_t instance_id_;

  ObjectLink();
  ObjectLink(uint16_t object_id, uint16_t instance_id);

  std::string toString() const;

  friend bool operator==(const ObjectLink& lhs, const ObjectLink& rhs);
};
} // namespace LwM2M

#endif //__LWM2M_OBJECT_LINK_DATA_TYPE_HPP