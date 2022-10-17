#include "CoAP_Binding.hpp"
#include "CoAP_ContentTypes.hpp"
#include "Message.hpp"
#include "Read.hpp"
#include "ReadComposite.hpp"
#include "TLV.hpp"
#include "Utility.hpp"

#include "HaSLL/LoggerManager.hpp"
#include "Variant_Visitor.hpp"

using namespace LwM2M;
using namespace std;
using namespace CoAP;
using namespace HaSLI;

#define ERROR_CODES_VALUE 0x80

CoAP_Binding::CoAP_Binding(
    const DeviceRegistryPtr& registry, const string& config_filepath)
    : CoAP_Binding(registry, getConfig(config_filepath)) {}

CoAP_Binding::CoAP_Binding(
    const DeviceRegistryPtr& registry, const CoAP::Configuration& config)
    : BindingInterface("CoAP", registry), Registrator(registry), Socket(config),
      encoder_(make_unique<CoAP_Encoder>()),
      decoder_(make_unique<CoAP_Decoder>()),
      logger_(LoggerManager::registerTypedLogger(this)) {
  logger_->log(SeverityLevel::TRACE, "Registering CoAP Server");

  SupportedContentFormats::addNewContentFormatType<
      ContentFormatEncodings::LwM2M_TLV>();
  SupportedContentFormats::addNewContentFormatType<
      ContentFormatEncodings::LwM2M_CBOR>();
  SupportedContentFormats::addNewContentFormatType<
      ContentFormatEncodings::LwM2M_JSON>();
}

CoAP_Binding::~CoAP_Binding() {
  LoggerManager::deregisterLogger(logger_->getName());
}

void CoAP_Binding::start() {
  Socket::start();
  logger_->log(SeverityLevel::INFO, "CoAP Server started!");
}

void CoAP_Binding::stop() {
  Socket::stop();
  logger_->log(SeverityLevel::INFO, "CoAP Server stoped!");
}

future<DataFormatPtr> CoAP_Binding::requestData(
    DeviceManagementRequestPtr request) {
  return async(
      launch::async,
      [this](DeviceManagementRequestPtr request) -> DataFormatPtr {
        auto coap_request = encodeRequest(request);
        logger_->log(SeverityLevel::TRACE,
            "Dispatching {}:{} as a data request to {}", request->name(),
            coap_request->getToken()->hexify(), request->endpoint_->toString());
        auto response_future = CoAP::Socket::request(coap_request);
        auto coap_response = response_future.get();
        logger_->log(SeverityLevel::TRACE,
            "Received a response with token {} to data request {} from {}",
            coap_response->getToken()->hexify(), request->name(),
            request->endpoint_->toString());
        auto response = decoder_->decode<ClientResponse>(coap_response);
        dispatched_.erase(std::hash<Message>{}(*request));
        if (response->response_code_ == ResponseCode::CONTENT) {
          if (response->payload_) {
            DataFormatPtr result;
            match(
                response->payload_->data_,
                [&](DataFormatPtr value) { result = value; },
                [&](TargetContentVector values) {
                  // Some read requests, that read from Multi-instance
                  // resources, MAY get a TargetContentVector response, instead
                  // of DataFormatPtr, because LwM2M_TLV sucks. Yes wakaama, I
                  // am talking about you. If this is the case, just take the
                  // first value, the entire thing is fucked anyways, might as
                  // well get some data out of it...
                  result = values[0].second;
                },
                [&](auto) {
                  logger_->log(SeverityLevel::ERROR,
                      "Response {} used an unexpected payload format for {}",
                      response->name(), request->name());
                });
            if (result) {
              return result;
            } else {
              string error_msg = "Could not decode response " +
                  response->name() + " payload into DataFormatPtr for " +
                  request->name();
              throw runtime_error(error_msg);
            }
          } else {
            throw ResponseReturnedAnEmptyPayload(response, request);
          }
        } else {
          throw ResponseReturnedAnErrorCode(response, request);
        }
      },
      request);
}

future<TargetContentVector> CoAP_Binding::requestMultiTargetData(
    DeviceManagementRequestPtr request) {
  return async(
      launch::async,
      [this](DeviceManagementRequestPtr request) -> TargetContentVector {
        auto coap_request = encodeRequest(request);
        logger_->log(SeverityLevel::TRACE,
            "Dispatching {}:{} as a multi target request to {}",
            request->name(), coap_request->getToken()->hexify(),
            request->endpoint_->toString());
        auto response_future = CoAP::Socket::request(coap_request);
        auto coap_response = response_future.get();
        logger_->log(SeverityLevel::TRACE,
            "Received a response with token {} to multi target "
            "request {} from {}",
            coap_response->getToken()->hexify(), request->name(),
            request->endpoint_->toString());
        auto response = decoder_->decode<ClientResponse>(coap_response);
        dispatched_.erase(std::hash<Message>{}(*request));
        if (response->response_code_ == ResponseCode::CONTENT) {
          if (response->payload_) {
            return std::get<TargetContentVector>(response->payload_->data_);
          } else {
            throw ResponseReturnedAnEmptyPayload(response, request);
          }
        } else {
          throw ResponseReturnedAnErrorCode(response, request);
        }
      },
      request);
}

future<bool> CoAP_Binding::requestAction(DeviceManagementRequestPtr request) {
  return async(
      launch::async,
      [this](DeviceManagementRequestPtr request) -> bool {
        auto coap_request = encodeRequest(request);
        logger_->log(SeverityLevel::TRACE,
            "Dispatching {}:{} as an action to {}", request->name(),
            coap_request->getToken()->hexify(), request->endpoint_->toString());
        auto response_future = CoAP::Socket::request(coap_request);
        auto coap_response = response_future.get();
        logger_->log(SeverityLevel::TRACE,
            "Received a response with token {} to an action {} from {}",
            coap_response->getToken()->hexify(), request->name(),
            request->endpoint_->toString());
        auto response = decoder_->decode<ClientResponse>(coap_response);
        dispatched_.erase(std::hash<Message>{}(*request));
        return static_cast<uint8_t>(response->response_code_) <
            ERROR_CODES_VALUE;
      },
      request);
}

future<ClientResponsePtr> CoAP_Binding::request(ServerRequestPtr request) {
  return async(
      launch::async,
      [this](ServerRequestPtr request) -> ClientResponsePtr {
        auto message = encodeRequest(request);
        logger_->log(SeverityLevel::TRACE,
            "Dispatching {}:{} as a generic request to {}", request->name(),
            message->getToken()->hexify(), request->endpoint_->toString());
        auto response_future = CoAP::Socket::request(message);
        auto coap_response = response_future.get();
        logger_->log(SeverityLevel::TRACE,
            "Received a response with token {} to generic request {} from {}",
            coap_response->getToken()->hexify(), request->name(),
            request->endpoint_->toString());
        auto response = decoder_->decode<ClientResponse>(coap_response);
        dispatched_.erase(std::hash<Message>{}(*request));
        return response;
      },
      request);
}

size_t CoAP_Binding::requestObservation(
    std::function<void(PayloadDataPtr)> notify_cb,
    InformationReportingRequestPtr request) {
  if (request) {
    auto message = encoder_->encode(request);
    auto message_id = message->getToken()->hash();
    auto observer = observed_elements_.find(message_id);
    if (observer == observed_elements_.end()) {
      auto response_future = CoAP::Socket::request(message);
      auto coap_response = response_future.get();
      auto response = decoder_->decode<ClientResponse>(coap_response);
      if (static_cast<uint8_t>(response->response_code_) < ERROR_CODES_VALUE) {
        observed_elements_.emplace(message_id, notify_cb);
        // @TODO: dispatch observation result, this is a bit of a problem,
        // because message_id must be first returned, before we can call the
        // notify_cb in a safe manner (maiking sure, that callback exists when
        // it is called)
        return message_id;
      } else {
        throw ResponseReturnedAnErrorCode(response, request);
      }
    } else {
      return observer->first;
    }
  } else {
    throw invalid_argument(
        "requestObservation must be called with a non empty ServerRequestPtr");
  }
}

void CoAP_Binding::cancelObservation(
    size_t observer_id, InformationReportingRequestPtr request) {
  auto observer = observed_elements_.find(observer_id);
  if (observer != observed_elements_.end()) {
    auto message = encoder_->encode(request);
    auto response_future = CoAP::Socket::request(message);
    auto coap_response = response_future.get();
    auto response = decoder_->decode<ClientResponse>(coap_response);
    if (static_cast<uint8_t>(response->response_code_) < ERROR_CODES_VALUE) {
      observed_elements_.erase(observer);
    } else {
      throw ResponseReturnedAnErrorCode(response, request);
    }
  } else {
    throw runtime_error("No Observer with id " + std::to_string(observer_id) +
        " was assigned for this Observable");
  }
}

void CoAP_Binding::cancelRequest(ServerRequestPtr request) {
  auto message = encodeRequest(request);
  if (request) {
    logger_->log(SeverityLevel::TRACE, "Canceling {}:{} for {}",
        request->name(), message->getToken()->hexify(),
        request->endpoint_->toString());
    CoAP::Socket::cancelRequest(message);
    logger_->log(SeverityLevel::TRACE, "{}:{} for {} canceled!",
        request->name(), message->getToken()->hexify(),
        request->endpoint_->toString());
  } else {
    logger_->log(SeverityLevel::ERROR, "Can not cancel an empty request!");
  }
}

void CoAP_Binding::handleNotification(const CoAP::MessagePtr& message) {
  auto observer = observed_elements_.find(message->getToken()->hash());
  if (observer != observed_elements_.end()) {
    logger_->log(SeverityLevel::TRACE,
        "Received an observe notification from {}:{} with token {}. "
        "Dispatching it to the Observer",
        message->getAddressIP(), message->getAddressPort(),
        message->getToken()->hexify());
    auto notification = decoder_->decode<ClientResponse>(message);
    if (auto payload = notification->payload_) {
      if (payload->hasData()) {
        observer->second(make_shared<PayloadData>(payload->data_));
      } else {
        logger_->log(SeverityLevel::WARNING,
            "Received an observe notification without data assigned "
            "to it's payload from {}:{} "
            "with token {}.",
            message->getAddressIP(), message->getAddressPort(),
            message->getToken()->hexify());
      }
    } else {
      logger_->log(SeverityLevel::WARNING,
          "Received an observe notification without payload from "
          "{}:{} with token {}.",
          message->getAddressIP(), message->getAddressPort(),
          message->getToken()->hexify());
    }
  } else {
    logger_->log(SeverityLevel::WARNING,
        "Received an orphaned observe notification from {}:{} with token {}.",
        message->getAddressIP(), message->getAddressPort(),
        message->getToken()->hexify());
  }
}

ServerResponsePtr CoAP_Binding::handleRegistrationRequest(
    const CoAP::MessagePtr& message) {
  auto options = message->getOptions();
  auto option = options.find(CoAP::OptionNumber::URI_PATH);
  if (option != options.end()) {
    if (option->second->getValueAsString() == "rd") {
      logger_->log(SeverityLevel::TRACE,
          "Handling incoming message from {}:{} as a Registration "
          "Interface message.",
          message->getAddressIP(), message->getAddressPort());
      try {
        if (message->getHeader()->getCodeType() == CoAP::CodeType::POST) {
          if (options.count(CoAP::OptionNumber::URI_PATH) > 1) {
            auto request = decoder_->decode<UpdateRequest>(message);
            return Registrator::handleRequest(request);
          } else {
            auto request = decoder_->decode<RegisterRequest>(message);
            return Registrator::handleRequest(request);
          }
        } else if (message->getHeader()->getCodeType() ==
            CoAP::CodeType::DELETE) {
          auto request = decoder_->decode<DeregisterRequest>(message);
          return Registrator::handleRequest(request);
        }
      } catch (RegistrationInterfaceError& ex) {
        auto endpoint = make_shared<Endpoint>(
            message->getAddressIP(), message->getAddressPort());
        return ex.response_;
      } catch (exception& ex) {
        logger_->log(SeverityLevel::CRITICAL,
            "Received an unhandled exception while decoding a "
            "registration request. Exception: {};",
            ex.what());
        auto endpoint = make_shared<Endpoint>(
            message->getAddressIP(), message->getAddressPort());
        return make_shared<RegisterResponse>(
            endpoint, ResponseCode::BAD_REQUEST);
      }
    }
  }
  return ServerResponsePtr();
}

ServerResponsePtr CoAP_Binding::handleRequest(const CoAP::MessagePtr& message) {
  logger_->log(SeverityLevel::TRACE,
      "Handling incoming message from {}:{} as a Request",
      message->getAddressIP(), message->getAddressPort());
  auto response = handleRegistrationRequest(message);
  if (response) {
    return response;
  }
  return ServerResponsePtr();
}

void CoAP_Binding::handleReceived(CoAP::MessagePtr message) {
  logger_->log(SeverityLevel::INFO, "Handling incoming message from {}:{}",
      message->getAddressIP(), message->getAddressPort());
  if (message->getHeader()->getMessageType() ==
      CoAP::MessageType::ACKNOWLEDGMENT) {
    logger_->log(SeverityLevel::WARNING,
        "Received an orphan response from {}:{} with ID {}",
        message->getAddressIP(), message->getAddressPort(),
        message->getToken()->hexify());
  } else if (message->getHeader()->getMessageType() ==
      CoAP::MessageType::NON_CONFIRMABLE) {
    handleNotification(message);
  } else if (message->getHeader()->getMessageType() ==
      CoAP::MessageType::CONFIRMABLE) {
    auto response = encoder_->encode(message, handleRequest(message));
    respond(response);
  }
}

CoAP::MessagePtr CoAP_Binding::encodeRequest(const ServerRequestPtr& request) {
  auto hash = std::hash<Message>{}(*request);
  auto it = dispatched_.find(hash);
  if (it == dispatched_.end()) {
    auto message = encoder_->encode(request);
    dispatched_.emplace(hash, message);
    return message;
  } else {
    return it->second;
  }
}
