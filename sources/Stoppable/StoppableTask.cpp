#include "StoppableTask.hpp"

using namespace std;

StoppableTask::StoppableTask() : StoppableTask(unique_ptr<Stoppable>(), "") {}

StoppableTask::StoppableTask(unique_ptr<Stoppable> task, string task_name)
    : task_(move(task)), task_thread_(make_unique<thread>()),
      task_name_(task_name) {}

void StoppableTask::startTask() {
  if (!task_thread_->joinable())
    task_thread_ = make_unique<thread>([&]() { task_->start(); });
  else {
    string error_msg = "Task" + task_name_ + " is already running";
    throw StoppableTaskIsAlreadyRunning(move(error_msg));
  }
}

void StoppableTask::stopTask() {
  if (task_thread_->joinable()) {
    task_->stop();
    task_thread_->join();
  } else {
    string error_msg = "Task" + task_name_ + " is not running";
    throw StoppableTaskIsNotRunning(move(error_msg));
  }
}

string StoppableTask::getName() { return task_name_; }
