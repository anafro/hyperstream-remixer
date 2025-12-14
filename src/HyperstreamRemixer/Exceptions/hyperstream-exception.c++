#include "hyperstream-exception.h++"

namespace HyperstreamRemixer::Exceptions {
HyperstreamException::HyperstreamException(const std::string &message) {
    this->message = message;
}

HyperstreamException::HyperstreamException(const char *message) {
    this->message = message;
}

HyperstreamException::~HyperstreamException() noexcept {
    // nothing to destroy //
}

const char *HyperstreamException::what() const noexcept {
    return this->message.c_str();
}
} // namespace HyperstreamRemixer::Exceptions
