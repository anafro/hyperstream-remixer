#pragma once
#include <exception>
#include <string>

namespace HyperstreamRemixer::Exceptions {
class HyperstreamException : public std::exception {
  public:
    explicit HyperstreamException(const std::string &message);
    explicit HyperstreamException(const char *message);
    ~HyperstreamException() noexcept override;

    auto what() const noexcept -> const char * override;

  protected:
    std::string message;
};
} // namespace HyperstreamRemixer::Exceptions
