#include "MessageProcessingThreadPool.hpp"
#include "CoAP_To_LwM2M.hpp"
#include "LwM2M_To_CoAP.hpp"

#include <cctype>
#include <optional>
#include <stdexcept>
#include <string>

using namespace std;
using namespace CoAP;
namespace LwM2M_Model {

MessageProcessingThreadPool::MessageProcessingThreadPool(
    shared_ptr<ThreadsafeQueue<CoAP_Message>> incoming_message_queue,
    shared_ptr<ThreadsafeQueue<LwM2M_Message>> outgoing_lwm2m_message_queue,
    shared_ptr<ThreadsafeQueue<CoAP_Message>> outgoing_message_queue)
    : outgoing_lwm2m_message_queue_(outgoing_lwm2m_message_queue),
      incoming_message_processing_thread_(make_unique<CoAP_To_LwM2M>(),
                                          incoming_message_queue,
                                          "IncomingMessageProcessor"),
      outgoing_message_processing_thread_(
          make_unique<LwM2M_To_CoAP>(outgoing_message_queue),
          outgoing_lwm2m_message_queue, "OutgoingMessageProcessor") {}

MessageProcessingThreadPool::~MessageProcessingThreadPool() {
  incoming_message_processing_thread_.processor_.stop();
  outgoing_message_processing_thread_.processor_.stop();
  // possible race condition at this point?
  incoming_message_processing_thread_.thread_id_.join();
  outgoing_message_processing_thread_.thread_id_.join();
}

} // namespace LwM2M_Model
