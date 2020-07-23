#ifndef __LWM2M_SERVER_HPP
#define __LWM2M_SERVER_HPP

#include "Logger.hpp"
#include "LwM2M_Device.hpp"
#include "Stoppable.hpp"

#include <memory>
#include <string>
#include <thread>
#include <unordered_map>
#include <vector>

namespace LwM2M {

class Process {
  std::unique_ptr<Stoppable> task_;
  std::unique_ptr<std::thread> task_thread_;
  std::string task_name_;

public:
  Process();
  Process(std::unique_ptr<Stoppable> task, std::string task_name);

  void startTask();
  void stopTask();
  std::string getName();
};

struct Configuration {
  std::string object_descriptors_location;
  std::string ip_address;
  unsigned int server_port;
  unsigned int read_timeout;
};

class Server : public Stoppable {
  std::vector<Process> processes_;
  std::shared_ptr<std::unordered_map<std::string, std::shared_ptr<Device>>>
      device_registery_;
  std::shared_ptr<HaSLL::Logger> logger_;
  void run() override;

public:
  Server();
  Server(Configuration config);

  void stop();
  std::shared_ptr<Device> getDevice(std::string device_id);
};
} // namespace LwM2M
#endif //__LWM2M_SERVER_HPP