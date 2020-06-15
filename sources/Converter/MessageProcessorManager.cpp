#include "MessageProcessorManager.hpp"
#include "CoAP_MessageConverter.hpp"
#include "LwM2M_MessageConverter.hpp"

#include <cctype>
#include <optional>
#include <stdexcept>
#include <string>

using namespace std;
using namespace CoAP;
namespace LwM2M_Model {

MessageProcessingManager::MessageProcessingManager(
    shared_ptr<ThreadsafeQueue<CoAP_Message>> incoming_message_queue,
    shared_ptr<ThreadsafeQueue<LwM2M_Message>> outgoing_lwm2m_message_queue,
    shared_ptr<ThreadsafeQueue<CoAP_Message>> outgoing_message_queue)
    : outgoing_lwm2m_message_queue_(outgoing_lwm2m_message_queue),
      incoming_message_processing_thread_(make_unique<LwM2M_MessageConverter>(),
                                          incoming_message_queue,
                                          "IncomingMessageProcessor"),
      outgoing_message_processing_thread_(
          make_unique<CoAP_MessageConverter>(outgoing_message_queue),
          outgoing_lwm2m_message_queue, "OutgoingMessageProcessor") {}

MessageProcessingManager::~MessageProcessingManager() {
  incoming_message_processing_thread_.processor_.stop();
  outgoing_message_processing_thread_.processor_.stop();
  // possible race condition at this point?
  incoming_message_processing_thread_.thread_id_.join();
  outgoing_message_processing_thread_.thread_id_.join();
}

} // namespace LwM2M_Model
