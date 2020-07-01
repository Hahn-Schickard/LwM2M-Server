#ifndef __LWM2M_SERVER_HPP
#define __LWM2M_SERVER_HPP

#include "LwM2M_Device.hpp"
#include "Stoppable.hpp"

#include <memory>
#include <string>
#include <thread>
#include <unordered_map>
#include <vector>

namespace LwM2M_Model {

struct LwM2M_Configuration {
  std::string object_descriptors_location;
  std::string ip_address;
  unsigned int server_port;
  unsigned int read_timeout;
};

class LwM2M_Server : public Stoppable {
  std::vector<std::unique_ptr<Stoppable>> processes_;
  std::vector<std::unique_ptr<std::thread>> process_threads_;
  std::shared_ptr<
      std::unordered_map<std::string, std::shared_ptr<LwM2M_Device>>>
      device_registery_;

  void run() override;

public:
  LwM2M_Server(LwM2M_Configuration config);

  void stop();
};
} // namespace LwM2M_Model
#endif //__LWM2M_SERVER_HPP