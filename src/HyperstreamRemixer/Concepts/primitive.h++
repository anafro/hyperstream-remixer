#pragma once
#include <concepts>

namespace HyperstreamRemixer::Concepts {
template <typename T>
concept numeric = std::floating_point<T> or std::integral<T>;
}
