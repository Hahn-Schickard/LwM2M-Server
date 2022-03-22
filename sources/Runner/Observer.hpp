#ifndef __LWM2M_EXAMPLE_OBSERVER_HPP
#define __LWM2M_EXAMPLE_OBSERVER_HPP

#include "Observable.hpp"

namespace LwM2M_Example {
using namespace LwM2M;

struct Observer : public ObserverInterface {
  Observer(ObservablePtr observable);

private:
  void handleEvent(PayloadDataPtr payload) override;
};

using ObserverPtr = std::shared_ptr<Observer>;
} // namespace LwM2M_Example
#endif //__LWM2M_EXAMPLE_OBSERVER_HPP