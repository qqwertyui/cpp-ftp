#pragma once

#include <cstddef>
#include <cstdint>

namespace ftp {
template <typename T> class Span {
  T *ptr_;
  std::size_t len_;

public:
  Span(T *ptr, std::size_t len) noexcept : ptr_{ptr}, len_{len} {}

  T &operator[](int i) noexcept { return *ptr_[i]; }
  T const &operator[](int i) const noexcept { return *ptr_[i]; }

  std::size_t size() const noexcept { return len_; }

  T *begin() noexcept { return ptr_; }
  T *end() noexcept { return ptr_ + len_; }
};

typedef Span<uint8_t> ByteSpan;
} // namespace ftp