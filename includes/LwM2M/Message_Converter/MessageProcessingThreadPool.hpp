#include "Converter.hpp"
#include "Stoppable.hpp"
#include "Threadsafe_Queue.hpp"

#include "CoAP_Message.hpp"
#include "LoggerRepository.hpp"
#include "LwM2M_Message.hpp"

#include <memory>
#include <thread>

namespace LwM2M_Model {
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
    run();
  }

  void run() override {
    while (!stopRequested()) {
      try {
        converter_->convert(message_buffer_->try_pop());
      } catch (std::exception &ex) {
        logger_->log(HaSLL::SeverityLevel::ERROR, "Caught an exception: {}",
                     ex.what());
      }
    }
  }
};

template <typename T> struct MessageProcessorThread {
  std::thread thread_id_;
  MessageProcessor<T> processor_;

  MessageProcessorThread() {}
  MessageProcessorThread(std::unique_ptr<Converter<T>> converter,
                         std::shared_ptr<ThreadsafeQueue<T>> message_buffer,
                         std::string logger_name)
      : processor_(move(converter), message_buffer, logger_name) {
    thread_id_ = std::thread([&]() { processor_.run(); });
  }
};

class MessageProcessingThreadPool {
  std::shared_ptr<ThreadsafeQueue<LwM2M_Message>> outgoing_lwm2m_message_queue_;
  MessageProcessorThread<CoAP::CoAP_Message>
      incoming_message_processing_thread_;
  MessageProcessorThread<LwM2M_Message> outgoing_message_processing_thread_;

public:
  MessageProcessingThreadPool(
      std::shared_ptr<ThreadsafeQueue<CoAP::CoAP_Message>>
          incoming_message_queue,
      std::shared_ptr<ThreadsafeQueue<LwM2M_Message>>
          outgoing_lwm2m_message_queue,
      std::shared_ptr<ThreadsafeQueue<CoAP::CoAP_Message>>
          outgoing_message_queue);
  ~MessageProcessingThreadPool();
};
} // namespace LwM2M_Model