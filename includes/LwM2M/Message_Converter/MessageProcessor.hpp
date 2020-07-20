#ifndef __LWM2M_MESSAGE_PROCESSOR_MANAGER_HPP
#define __LWM2M_MESSAGE_PROCESSOR_MANAGER_HPP

#include "Converter.hpp"
#include "LoggerRepository.hpp"
#include "Stoppable.hpp"
#include "Threadsafe_Queue.hpp"

#include <memory>
#include <thread>

namespace LwM2M {
template <typename T> class MessageProcessor : public Stoppable {
  std::unique_ptr<Converter<T>> converter_;
  std::shared_ptr<ThreadsafeQueue<T>> message_buffer_;
  std::shared_ptr<HaSLL::Logger> logger_;

public:
  MessageProcessor() {}

  MessageProcessor(std::unique_ptr<Converter<T>> converter,
                   std::shared_ptr<ThreadsafeQueue<T>> message_buffer,
                   std::string logger_name)
      : Stoppable(), converter_(move(converter)),
        message_buffer_(message_buffer),
        logger_(
            HaSLL::LoggerRepository::getInstance().registerLoger(logger_name)) {
  }

  void run() override {
    while (!stopRequested()) {
      try {
        converter_->convert(message_buffer_->wait_and_pop());
      } catch (std::exception &ex) {
        logger_->log(HaSLL::SeverityLevel::ERROR, "Caught an exception: {}",
                     ex.what());
      }
    }
  }
};
} // namespace LwM2M

#endif //__LWM2M_MESSAGE_PROCESSOR_MANAGER_HPP