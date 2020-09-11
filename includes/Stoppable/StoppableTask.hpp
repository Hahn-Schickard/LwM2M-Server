#ifndef __MULTITHREADING_STOPABLE_TASK_HPP
#define __MULTITHREADING_STOPABLE_TASK_HPP

#include "Stoppable.hpp"

#include <memory>
#include <stdexcept>
#include <string>
#include <thread>

struct StoppableTaskIsAlreadyRunning : public std::runtime_error {
  StoppableTaskIsAlreadyRunning(std::string const &message)
      : std::runtime_error(message) {}
};

struct StoppableTaskIsNotRunning : public std::runtime_error {
  StoppableTaskIsNotRunning(std::string const &message)
      : std::runtime_error(message) {}
};

class StoppableTask {
  std::unique_ptr<Stoppable> task_;
  std::unique_ptr<std::thread> task_thread_;
  std::string task_name_;

public:
  StoppableTask();
  StoppableTask(std::unique_ptr<Stoppable> task, std::string task_name);

  void startTask();
  void stopTask();
  std::string getName();
};

#endif //__MULTITHREADING_STOPABLE_TASK_HPP