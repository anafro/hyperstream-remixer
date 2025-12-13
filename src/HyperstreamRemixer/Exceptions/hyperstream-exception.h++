#pragma once
#include <string>
#include <exception>

namespace HyperstreamRemixer::Exceptions {
    class HyperstreamException : public std::exception {
    public:
        explicit HyperstreamException(const std::string& message);
        explicit HyperstreamException(const char* message);
        ~HyperstreamException() noexcept override;

        const char* what() const noexcept override;
    protected:
        std::string message;
    };
}
