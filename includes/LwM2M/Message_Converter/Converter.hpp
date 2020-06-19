#ifndef __LWM2M_MESSAGE_CONVERTER_HPP
#define __LWM2M_MESSAGE_CONVERTER_HPP
#include <memory>

namespace LwM2M_Model {

template <typename T> class Converter {
public:
  virtual ~Converter() = default;
  virtual void convert(std::shared_ptr<T> message) = 0;
};

} // namespace LwM2M_Model

#endif //__LWM2M_MESSAGE_CONVERTER_HPP