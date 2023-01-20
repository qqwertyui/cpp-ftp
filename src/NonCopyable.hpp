#pragma once

namespace ftp {
class NonCopyable {
public:
  NonCopyable() = default;
  NonCopyable(NonCopyable &) = delete;
};
} // namespace ftp